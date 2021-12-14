#pragma once
#include "Search.h"
#include "VCF/VCFsolver.h"
const int MAX_MCTS_CHILDREN = 50;
const double policyQuant = 50000;

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
  
  MCTSnode(Evaluator* evaluator,  Color nextColor,Loc* locbuf,PolicyType* pbuf1,PolicyType* pbuf2,float* pbuf3);
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
    float expandFactor = 0.7;
    float puct = 1.0;

  }params;

  VCFsolver vcfSolver[2];

  Loc locbuf[BS * BS];
  PolicyType pbuf1[BS * BS], pbuf2[MAX_MCTS_CHILDREN];
  float pbuf3[MAX_MCTS_CHILDREN];


  MCTSsearch(Evaluator *e);
  virtual float    fullsearch(Color color, double factor, Loc &bestmove);
  virtual void play(Color color, Loc loc);
  virtual void undo( Loc loc);

  void             setOptions(size_t maxNodes) { option.maxNodes = maxNodes; }
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


  Loc bestRootMove();
  float rootValue();


};

