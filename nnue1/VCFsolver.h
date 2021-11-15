#pragma once
#include "global.h"
namespace VCF {

struct alignas(32) PT
{
  int16_t loc1, loc2;
};
inline Loc xytoshapeloc(int x, int y) { return Loc((BS + 3) * y + x + 3); }

enum PlayResult : int16_t {
  PR_WIN,                  //�ڣ�˫�Ļ�������  �ף�˫�Ļ�������
  PR_OneFourWithThree,     //�ڣ���������ͬʱ�������� �ף�������
  PR_OneFourWithoutThree,  //�ڣ���������ͬʱ���������� �ף�������
  PR_NORMAL,               //�ڣ������� �ף������ķ���
  PR_LOSE                  //�ڣ����Ϸ�(��ץ��)����û�����ɳ��� �ף����Ϸ�(��ץ��)
};

const Loc dirs[4] = { Loc(1), Loc(BS + 6), Loc(BS + 6 + 1), Loc(-BS - 6 + 1) };//+x +y +x+y +x-y

}  // namespace VCF

class VCFsolver
{
public:
  const int H, W;

  Color board[(BS + 6) * (BS + 6)];  //Ԥ��3Ȧ
  // shape=1*��������+6*����+256*�Է�����-16384*����
  // �������ҽ���shape=3�����ĵ��ҽ���shape=4�����ֳ��ĵ��ҽ���shape==256*4
  int16_t shape[4][(BS + 6) * (BS + 6)];  //Ԥ��3Ȧ

  int     ptNum;             // pts����ǰ���ٸ���Ч
  VCF::PT pts[8 * BS * BS];  //����


  VCFsolver();

  //����board
  //b���ⲿ�����̣�pla�ǽ�����
  //katagoType�Ƿ���katago�����̣�false��Ӧloc=x+y*BS��true��Ӧloc=x+1+y*(BS+1)
  void setBoard(Color* b,Color pla,bool katagoType);
  void setBoard2(Color* b);


  //Ϊ�˷��㣬����������Ǻڣ����ط��ǰס�
  VCF::PlayResult playB(Loc loc, Loc &nextForceLoc);
  VCF::PlayResult playW(Loc loc, Loc &nextForceLoc);
  
};
