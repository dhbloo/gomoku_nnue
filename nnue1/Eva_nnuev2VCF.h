#pragma once
#include "Eva_nnuev2.h"
#include "EvaluatorOneSide.h"
#include "VCF/VCFsolver.h"

#include <vector>

class Eva_nnuev2VCF : public Eva_nnuev2
{
public:
  VCFsolver vcfsolver;

  VCF::SearchResult vcfResult;  //���֮ǰ���VCF�ͼ�¼������
  Loc               vcfWinLoc;  //���֮ǰ���VCF�ͼ�¼������

  virtual bool loadParam(std::string filepath);
  virtual void clear();
  virtual void recalculate();  //����board��ȫ���¼������α�

  //������Ϊ�����֣���һ�����ǿ���������ģ�����play������ڶ������ǲ�����������ģ�����evaluate�
  virtual void      play(Color color, Loc loc);
  virtual ValueType evaluateFull(PolicyType *policy);    // policyͨ��������������
  virtual void      evaluatePolicy(PolicyType *policy);  // policyͨ��������������
  virtual ValueType evaluateValue();                     //

  virtual void undo(Loc loc);  // play�������

  // virtual void debug_print();
};
