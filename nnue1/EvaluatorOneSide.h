#pragma once
#include "global.h"
class EvaluatorOneSide
{
public:
  Color mySide;//�����ֵ�����ıߵġ�������һ����˭�ߵģ���������һ��ΪmySide�Ĺ�ֵ
  Color board[BS * BS];
  virtual bool loadParam(std::string filepath) = 0;
  virtual void clear() = 0;
  virtual void recalculate() = 0;//����board��ȫ���¼������α�
  
  //������Ϊ�����֣���һ�����ǿ���������ģ�����play������ڶ������ǲ�����������ģ�����evaluate�
  virtual void      play(Color color, Loc loc)   = 0;
  virtual ValueType evaluateFull(PolicyType *policy) = 0;  // policyͨ��������������
  virtual void evaluatePolicy(PolicyType *policy) = 0;  // policyͨ��������������
  virtual ValueType evaluateValue()  = 0;  //

  virtual void undo(Loc loc) = 0;//play�������
  virtual void debug_print() {}//debug��
};


