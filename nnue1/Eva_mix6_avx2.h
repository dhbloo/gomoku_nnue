#pragma once
#include <vector>
#include "EvaluatorOneSide.h"

namespace mix6
{
  const int shapeNum = 708588;
  const int featureNum = 48;
  const int featureBatch = featureNum / 16;
  const int policyNum = 16;
  const int policyBatch = policyNum / 16;
  const int valueNum = 32;
  const int valueBatch = valueNum / 16;
  const int valueBatch32 = valueNum / 8;
}

/*
�������̣�
1.��������id��������feature����

*/
struct Mix6weight_int16
{
  static const int16_t IllegalShapeFeature = 11454;//>6000
  //1  map=weight.map(board), shape=H*W*4*c
  int16_t map[mix6::shapeNum][mix6::featureNum];

  //2  mapsum=map.sum(2), shape=HWc
  
  //3  mapAfterLR=leakyRelu(mapsum)
  int16_t map_lr_slope_sub1div8[mix6::featureNum];//��ȥ1�������  0.25f(x)=x>>2+min(x,0)*slopeSub1Div8*2
  int16_t map_lr_bias[mix6::featureNum];

  //4  update policyBeforeConv and valueSumBoard

  //5  policyAfterConv=depthwise_conv2d(policyBeforeConv)
  int16_t policyConvWeight[9][mix6::policyNum];
  int16_t policyConvBias[mix6::policyNum];

  //6  policy=conv1x1(relu(policyAfterConv))
  int16_t policyFinalConv[mix6::policyNum];
  //7  policy=leakyRelu(policyAfterConv)
  float policy_neg_slope, policy_pos_slope;

  //8  value leakyRelu
  float scale_beforemlp;
  float value_lr_slope_sub1[mix6::valueNum];
  
  //9  mlp
  float mlp_w1[mix6::valueNum][mix6::valueNum];//shape=(inc��outc)����ͬ��inc��ӦȨ������
  float mlp_b1[mix6::valueNum];
  float mlp_w2[mix6::valueNum][mix6::valueNum];
  float mlp_b2[mix6::valueNum];
  float mlp_w3[mix6::valueNum][3];
  float mlp_w3_for_safety[5];//mlp_w3��read��ʱ��һ��read 8������read�������ڴ�mlp_w3[mix6::valueNum-1][2]+5��
  float mlp_b3[3];
  float mlp_b3_for_safety[5];//mlp_b3��read��ʱ��һ��read 8������read�������ڴ�mlp_b3[2]+5��


  bool loadParam(std::string filename);

};
struct OnePointChange
{
  Loc loc;
  int16_t dir;
  uint32_t oldshape;
  uint32_t newshape;
};
struct Mix6buf_int16
{
  //0 convert board to shape
  uint32_t shapeTable[BS * BS][4];//4������BS*BS��λ��

  //1  map=weight.map(shape), shape=H*W*4*c
  //2  mapsum=map.sum(2), shape=HWc
  int16_t mapsum[BS * BS][mix6::featureNum];

  //3  mapAfterLR=leakyRelu(mapsum)
  int16_t mapAfterLR[BS * BS][mix6::featureNum];

  //4  policyBeforeConv=mapAfterLR[policy channels]
  //5  policyAfterConv=depthwise_conv2d(policyBeforeConv)
  int16_t policyAfterConv[BS * BS][mix6::policyNum];

  //6  policy=conv1x1(Relu(policyAfterConv))

  //7  valueSumBoard=mapAfterLR[value channels].sum(0,1)     
  //��int32�������
  int32_t valueSumBoard[mix6::valueNum];


  //8  ������mlp������Ҫ����
  void update(Color oldcolor, Color newcolor, Loc loc, const Mix6weight_int16& weights);
  ValueType calculateValue(const Mix6weight_int16& weights);//mlp
  void calculatePolicy(PolicyType* policy, const Mix6weight_int16& weights);

  void emptyboard(const Mix6weight_int16& weights);//init


};
class Eva_mix6_avx2 :
    public EvaluatorOneSide
{
public:
  uint64_t Total_Eval_Num;
  Mix6weight_int16 weights;
  Mix6buf_int16 buf;

  virtual bool loadParam(std::string filepath);
  virtual void clear();
  virtual void recalculate();//����board��ȫ���¼������α�

  //������Ϊ�����֣���һ�����ǿ���������ģ�����play������ڶ������ǲ�����������ģ�����evaluate�
  virtual void play(Color color, Loc loc);
  virtual ValueType evaluate(PolicyType* policy);//policyͨ��������������

  virtual void undo(Loc loc);//play�������

  virtual void debug_print();
};

