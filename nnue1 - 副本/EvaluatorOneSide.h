#pragma once
#include "global.h"
class EvaluatorOneSide
{
public:
  Color mySide;//�����ֵ�����ıߵġ�������һ����˭�ߵģ���������һ��ΪmySide�Ĺ�ֵ
  Color board[BS * BS];
  virtual bool loadParam(std::ifstream& fs) = 0;
  virtual void clear() = 0;
  virtual void recalculate() = 0;//����board��ȫ���¼������α�
  
  //������Ϊ�����֣���һ�����ǿ���������ģ�����play������ڶ������ǲ�����������ģ�����evaluate�
  virtual void play(uint8_t color, uint16_t loc) = 0;
  virtual double evaluate(PolicyType* policy) = 0;//policyͨ��������������

  virtual void undo(uint16_t loc) = 0;//play�������
  virtual void debug_print() {}//debug��
};


