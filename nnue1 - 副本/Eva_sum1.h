/*
* ��򵥵�
* ����ӳ�����
* ����9
*/


#pragma once
#include "EvaluatorOneSide.h"
const int32_t pow3[] = { 1,3,9,27,81,243,729,2187,6561,19683,59049 };
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
  * ���С�ķ����ϻ������������������ң�Ϊ��λ�����磬x-------- = 000000001, --------x = 100000000
  * ������ǽ�����λ���ϼ���1�����磬--------# = 1000000000��-------x# = 1010000000��-------## = 1100000000��------x## = 1101000000
  * ������ǽ�����λ����2,������Ϊk�������λ����k-1��1�����磬#-------- = 2000000000��#x------- = 2000000010��##------- = 2000000001��##x------ = 2000000101��###x----- = 2000001021
  * �������߶���ǽ�����λ����3������ұ߶����ϡ����-1����1�����磬#-------# = 3000000000��##------# = 3000000001��###----## = 3100000011
  * ���ϱ���������ȷ������ͨ��������λ������ʵ���������㡣
  * ������Ϊ4*3^9=78732
  */
  static const int shapeNum = 78732;
  static const double quantFactor;//��������=32
  int32_t weight[4][shapeNum];//4�ֱ��ǣ�policy��ʤ�������ͣ�
  static const uint32_t Weight_No_Implement = 114514;
  virtual bool loadParam(std::ifstream& fs);
  virtual void clear();
  virtual void recalculate();//����board��ȫ���¼������α�

  //������Ϊ�����֣���һ�����ǿ���������ģ�����play������ڶ������ǲ�����������ģ�����evaluate�
  virtual void play(Color color, Loc loc);
  virtual double evaluate(PolicyType* policy);//policyͨ��������������

  virtual void undo(Loc loc);//play�������

  virtual void debug_print();


private:
  void initShapeTable();
  double getWinlossRate();
};

