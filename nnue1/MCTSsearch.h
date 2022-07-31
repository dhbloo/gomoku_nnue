#pragma once
#include "Search.h"
#include "VCF/VCFsolver.h"
const int MAX_MCTS_CHILDREN = 50;
const double policyQuant = 50000;
const double policyQuantInv = 1/policyQuant;

enum MCTSsureResult : int16_t {
  MC_Win = 1,
  MC_LOSE = -1,
  MC_DRAW = 2,
  MC_UNCERTAIN = 0
};
inline double sureResultWR(MCTSsureResult sr)
{
  if (sr == MC_Win)return 1;
  else if (sr == MC_LOSE)return -1;
  else return 0;
}

inline double MCTSpuctFactor(double totalVisit, double puct, double puctPow, double puctBase)
{
  return  puct * pow((totalVisit+puctBase)/puctBase, puctPow);
}

inline double MCTSselectionValue(double puctFactor, double value,double childVisit,double childPolicy)
{
  return value + puctFactor * childPolicy / (childVisit + 1);
}

struct MCTSnode;

struct MCTSchild
{
  MCTSnode* ptr;
  Loc loc;
  uint16_t policy;//����policyQuant��ԭʼpolicy
};

struct MCTSnode
{
  MCTSsureResult sureResult;
  int16_t childrennum;
  int16_t legalChildrennum;//=min(�Ϸ��з���MAX_MCTS_CHILDREN)
  MCTSchild children[MAX_MCTS_CHILDREN];
  

  uint64_t visits;//�����Լ�
  double WRtotal;//����һ�����ӵ���һ�����ӽ�,1ʤ-1��
  //ƽ��ʤ��=WRtotal/visits

  Color nextColor;
  
  MCTSnode(Evaluator* evaluator,  Color nextColor,double policyTemp,Loc* locbuf,PolicyType* pbuf1,PolicyType* pbuf2,float* pbuf3);
  MCTSnode(MCTSsureResult sureResult, Color nextColor);
  ~MCTSnode();
  
};

class MCTSsearch :
    public Search
{
public:
  MCTSnode* rootNode;

  struct Option
  {
    size_t maxNodes=0;
  } option;
  struct Param
  {
    double expandFactor = 0.2;//��ͳ��mcts��Ӧ0.0, Ҳ����ÿ��playout�̶�����1��Ҷ�ӽڵ�
    double puct = 2;
    double puctPow = 0.75;//��ͳ��mcts��Ӧ0.5
    double puctBase = 10;
    double fpuReduction = 0.1;
    double policyTemp = 1.1;

  }params;

  VCFsolver vcfSolver[2];

  Loc locbuf[BS * BS];
  PolicyType pbuf1[BS * BS], pbuf2[MAX_MCTS_CHILDREN];
  float pbuf3[MAX_MCTS_CHILDREN];


  MCTSsearch(Evaluator *e);
  virtual float    fullsearch(Color color, double factor, Loc &bestmove);
  virtual void play(Color color, Loc loc);
  virtual void undo( Loc loc);
  virtual void clearBoard(); 

  Loc bestRootMove() const;
  float getRootValue() const;
  int64_t getRootVisit() const;

  void             setOptions(size_t maxNodes) { option.maxNodes = maxNodes; }
  void loadParamFile(std::string filename);
  ~MCTSsearch() { if(rootNode!=NULL)delete rootNode; }

private:
  struct SearchResult
  {
    uint64_t newVisits;
    double WRchange;
  };
  void playForSearch(Color color, Loc loc);
  void undoForSearch(Loc loc);

  SearchResult search(MCTSnode* node, uint64_t remainVisits,bool isRoot);
  MCTSsureResult checkSureResult(Loc nextMove, Color color);//check VCF
  int selectChildIDToSearch(MCTSnode* node);



};

