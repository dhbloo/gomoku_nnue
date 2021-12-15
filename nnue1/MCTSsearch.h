#pragma once
#include "Search.h"
#include "VCF/VCFsolver.h"
const int    MAX_MCTS_CHILDREN = 50;
const double policyQuant       = 50000;
const double policyQuantInv    = 1 / policyQuant;

enum MCTSsureResult : int16_t { MC_Win = 1, MC_LOSE = -1, MC_DRAW = 2, MC_UNCERTAIN = 0 };
inline double sureResultWR(MCTSsureResult sr)
{
  if (sr == MC_Win)
    return 1;
  else if (sr == MC_LOSE)
    return -1;
  else
    return 0;
}

inline double
MCTSpuctFactor(double totalVisit, double puct, double puctPow, double puctBase)
{
  return puct * pow((totalVisit + puctBase) / puctBase, puctPow);
}

inline double
MCTSselectionValue(double puctFactor, double value, double childVisit, double childPolicy)
{
  return value + puctFactor * childPolicy / (childVisit + 1);
}

struct MCTSnode;

struct int128_t
{
  int64_t data[2];
};

struct MCTSchild
{
  int32_t  offset;
  Loc      loc;
  uint16_t policy;  // ����policyQuant��ԭʼpolicy

  MCTSnode *ptr() const
  {
    if (offset)
      return reinterpret_cast<MCTSnode *>(base_ptr() + offset);
    else
      return nullptr;
  }
  void setPtr(MCTSnode *ptr)
  {
    // ���ڿ�ָ�룬offsetֱ�Ӵ�0
    if (ptr == nullptr) {
      offset = 0;
      return;
    }

    // ����MCTSnode�Ѿ���16byte����ģ���ת��û����
    int128_t *target = reinterpret_cast<int128_t *>(ptr);
    ptrdiff_t diff   = target - base_ptr();
    offset           = (int32_t)diff;
    // �����ڴ�����������ģ����η���ľ��볬��32λoffset��ʾ��Χ:
    // 2^32 * sizeof(int128_t) = 64GB ����������ܷ���
    // ����û��Ǽ��һ��
    if (offset != diff) {
      std::cerr << "You are very unlucky!!" << std::endl;
      std::terminate();
    }
  }

private:
  // ��ö��뵽int64_t�Ļ�׼ָ��
  int128_t *base_ptr() const
  {
    intptr_t this_ptr = reinterpret_cast<intptr_t>(this);
    return reinterpret_cast<int128_t *>(this_ptr & (~15));
  }
};

struct alignas(int128_t) MCTSnode
{
  MCTSsureResult sureResult;
  int16_t        childrennum;
  int16_t        legalChildrennum;  //=min(�Ϸ��з���MAX_MCTS_CHILDREN)
  Color          nextColor;
  uint64_t       visits;  //�����Լ�
  double WRtotal;  //����һ�����ӵ���һ�����ӽ�,1ʤ-1��, ƽ��ʤ��=WRtotal/visits
  MCTSchild children[MAX_MCTS_CHILDREN];

  MCTSnode(Evaluator * evaluator,
           Color       nextColor,
           double      policyTemp,
           Loc *       locbuf,
           PolicyType *pbuf1,
           PolicyType *pbuf2,
           float *     pbuf3);
  MCTSnode(MCTSsureResult sureResult, Color nextColor);
  ~MCTSnode();
};

const int a = sizeof(MCTSnode);

class MCTSsearch : public Search
{
public:
  MCTSnode *rootNode;

  struct Option
  {
    size_t maxNodes = 0;
  } option;
  struct Param
  {
    double expandFactor = 0.4;
    double puct         = 0.6;
    double puctPow      = 0.7;  //��ͳ��mcts��Ӧ0.5
    double puctBase     = 1.0;
    double fpuReduction = 0.1;
    double policyTemp   = 1.1;

  } params;

  VCFsolver vcfSolver[2];

  Loc        locbuf[BS * BS];
  PolicyType pbuf1[BS * BS], pbuf2[MAX_MCTS_CHILDREN];
  float      pbuf3[MAX_MCTS_CHILDREN];

  MCTSsearch(Evaluator *e);
  virtual float fullsearch(Color color, double factor, Loc &bestmove);
  virtual void  play(Color color, Loc loc);
  virtual void  undo(Loc loc);
  virtual void  clearBoard();

  Loc     bestRootMove() const;
  float   getRootValue() const;
  int64_t getRootVisit() const;

  void setOptions(size_t maxNodes) { option.maxNodes = maxNodes; }
  void loadParamFile(std::string filename);
  ~MCTSsearch()
  {
    if (rootNode != NULL)
      delete rootNode;
  }

private:
  struct SearchResult
  {
    uint64_t newVisits;
    double   WRchange;
  };
  void playForSearch(Color color, Loc loc);
  void undoForSearch(Loc loc);

  SearchResult   search(MCTSnode *node, uint64_t remainVisits, bool isRoot);
  MCTSsureResult checkSureResult(Loc nextMove, Color color);  // check VCF
  int            selectChildIDToSearch(MCTSnode *node);
};
