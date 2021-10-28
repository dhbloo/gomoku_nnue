#pragma once
#include "global.h"
#include "Search.h"
class PolicyABsearch
  :public Search
{
public:
  PolicyABsearch(Evaluator* e) :Search(e),currentDepth(0) {}
  virtual double fullsearch(Color color, double factor, Loc& bestmove);
private:
  int currentDepth;
  double searchRec(Color color, double remainPolicy, double maxEval, double minEval, Loc& bestmove);//����һ����֧����maxeval��ֱ�ӷ��أ�����ab��֦����minEval���ݵ��²�

  int explorationFactor(int depth);//������depth�㣬̽��policy�½�������explorationFactor��ѡ��
  int minExploreChildren(int depth);//������depth�㣬�����Ѷ���ѡ��
  bool isWin(Color color, Loc toplayLoc);

  int policyRankBuf[BS * BS][BS * BS];//policyRankBuf[depth][loc],�����ӽڵ��policy�Ѹ��ڵ㸲��
  PolicyType policyBuf[BS * BS][BS * BS];
  void sortPolicy(const PolicyType* policy, int* policyRank);
};

