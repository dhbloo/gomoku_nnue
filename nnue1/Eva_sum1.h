/*
* ��򵥵�
* ����ӳ�����
* ����9
*/


#pragma once
#include "EvaluatorOneSide.h"
const int32_t pow3[] = { 1,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441 };
class Eva_sum1 :
    public EvaluatorOneSide
{
public:
  uint64_t Total_Eval_Num;
  uint32_t shapeTable[4][BS * BS];//4������BS*BS��λ��
  int64_t winSum, lossSum, drawSum;
  PolicyType policyBuf[BS * BS];


  /*
  * ���α������
  * x��ʾ������o��ʾ�Է���-��ʾ�հף�#��ʾǽ��*��ʾ����
  * �����Ʊ�ʾ������1���Է�2���հ�0
  * Ϊ�˷��㣬���µ������������Ʊ�ʾ
  * ���С�ķ����ϻ������������������ң�Ϊ��λ�����磬x---------- = 00000000001, ----------x = 10000000000
  * ������ǽ�����λ���ϼ���1�����磬----------# = 100000000000��---------x# = 101000000000��---------## = 110000000000��--------x## = 110100000000
  * ������ǽ�����λ����2,������Ϊk�������λ����k-1��1�����磬#---------- = 200000000000��#x--------- = 200000000010��##--------- = 200000000001��##x-------- = 200000000101��###x------- = 200000001021
  * �������߶���ǽ�����λ����3������ұ߶����ϡ����-1����1�����磬#---------# = 300000000000��##--------# = 300000000001��###------## = 310000000011
  * ���ϱ���������ȷ������ͨ��������λ������ʵ���������㡣
  * ������Ϊ4*3^11=708588
  */
  static const int shapeNum = 708588;
  int32_t weight[4][shapeNum];//4�ֱ��ǣ�policy��ʤ�������ͣ�
  static const uint32_t Weight_No_Implement = 114514;
  virtual bool loadParam(std::ifstream& fs);
  virtual void clear();
  virtual void recalculate();//����board��ȫ���¼������α�

  //������Ϊ�����֣���һ�����ǿ���������ģ�����play������ڶ������ǲ�����������ģ�����evaluate�
  virtual void play(Color color, Loc loc);
  virtual ValueType evaluate(PolicyType* policy);//policyͨ��������������

  virtual void undo(Loc loc);//play�������

  virtual void debug_print();


private:
  void initShapeTable();
  double getWinlossRate();
};

