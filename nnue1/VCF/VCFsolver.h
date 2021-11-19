#pragma once
#include "..\global.h"
namespace VCF {

  struct alignas(64) PT
  {
    int16_t shapeDir;
    Loc shapeLoc,loc1, loc2;
    PT():shapeDir(0), shapeLoc(LOC_NULL),loc1(LOC_NULL),loc2(LOC_NULL){}
    //PT(Loc loc1,Loc loc2):loc1(loc1),loc2(loc2){}
  };
  inline Loc xytoshapeloc(int x, int y) { return Loc((BS + 6) * (y + 3) + x + 3); }

  enum PlayResult : int16_t {
    PR_Win,                  //˫�Ļ����������ץ��
    PR_OneFourWithThree,     //��������ͬʱ��������
    PR_OneFourWithTwo,     //��������ͬʱ�����߶�������������
    PR_OneFourWithoutTwo,  //��������ͬʱ�������߶�����
    PR_Lose                  //���Ϸ�(��ץ��)����û�����ɳ���
  };
  enum SearchResult : int16_t {
    SR_Win = -1,//�н�
    SR_Uncertain = 0,//��ȫ��֪����û�н⣬��û��
    SR_Lose = 32767//ȷ���޽�
    //����ֵ������ʱ�޽⣬��SearchResult=n��˵��������n-1�����������ĳ����޽⡱
  };


  static const Loc dirs[4] = { 1, BS + 6, BS + 6 + 1, -BS - 6 + 1 };//+x +y +x+y +x-y
  //Ϊ�˷��㳣�����ã��ֿ���дһ��
  static const Loc dir0 = 1;
  static const Loc dir1 =  BS + 6;
  static const Loc dir2 =  BS + 6 + 1;
  static const Loc dir3 = -BS - 6 + 1;
  static const int ArrSize = (BS + 6) * (BS + 6);//���Ƕ���3Ȧ������̸����


}  // namespace VCF

class VCFsolver
{
public:
  const int H, W;
  const bool isWhite;//����������Ǻ��壬��false���������ǰ�����true����true��colorȫ�Ƿ���

  Color board[(BS + 6) * (BS + 6)];  //Ԥ��3Ȧ
  // shape=1*��������+8*����+64*�Է�����+512*���ֳ���+4096*����
  int16_t shape[4][(BS + 6) * (BS + 6)];  //Ԥ��3Ȧ

  int     ptNum;             // pts����ǰ���ٸ���Ч
  VCF::PT pts[8 * BS * BS];  //����

  int64_t nodeNumThisSearch;//��¼��������Ѿ������˶��ٽڵ��ˣ�������ǰ��ֹ��ÿ��fullSearch��ʼʱ����
  int movenum;//�������ӿ�ʼvcf�����ڵľ����Ѿ���������
  Loc PV[BS * BS];//��¼·��
  int PVlen;

  VCFsolver(Color pla) :VCFsolver(BS, BS,pla) {}
  VCFsolver(int h, int w,Color pla);
  void reset();

  //����board
  //b���ⲿ�����̣�pla�ǽ�����
  //katagoType�Ƿ���katago�����̣�false��Ӧloc=x+y*BS��true��Ӧloc=x+1+(y+1)*(W+1)
  //colorType��Դ���̵ı����ʽ��=false �����Է���=true ��ɫ��ɫ
  void setBoard(Color* b, bool katagoType, bool colorType);


  VCF::SearchResult fullSearch(float factor, Loc& bestmove, bool katagoType);//factor��������������֤factor�����ڽڵ�����
  inline int getPVlen() { return PVlen; };
  std::vector<Loc> getPV();//�Ƚ���
  std::vector<Loc> getPVreduced();//��������PV

  
  //�����ⲿ���ã��������̡���֤shape��ȷ������֤pts��ȷ��
  //locType=0 vcf�ڲ�loc��ʽ��=1 x+y*BS��=2 katago��ʽ
  //colorType=false �����Է���=true ��ɫ��ɫ
  void playOutside(Loc loc, Color color, int locType,bool colorType);
  void undoOutside(Loc loc, int locType);//�����ⲿ���ã��������̡���֤shape��ȷ������֤pts��ȷ��

  //debug
  void printboard();

private:

  //����pts����������ǰһ�����������
  //ͬʱ��鼺�����Է���û���������
  VCF::SearchResult resetPts(Loc& onlyLoc);

  //�������յ�
  VCF::PT findEmptyPoint2(Loc loc,int dir);
  //��һ���յ�
  Loc findEmptyPoint1(Loc loc,Loc bias);//bias=dirs[dir]

  //��һ���غϣ��ȳ����ٷ���
  //loc1�ǹ���loc2���أ�nextForceLoc�ǰ������
  VCF::PlayResult playTwo(Loc loc1,Loc loc2, Loc& nextForceLoc);
  void undo(Loc loc);
  //maxNoThree�����maxNoThree��û���������������ַ��ĵĳ��Ĳ���
  //forceLoc����һ�������������Ϊ�����г���
  //winLoc�����ػ�ʤ��
  //ptNumOld����һ����������������֮��ԭ
  VCF::SearchResult search(int maxNoThree, Loc forceLoc);

  // shape=1*��������+8*����+64*�Է�����+512*���ֳ���+4096*����
  inline bool shape_isMyFive(int16_t s) { return (s & 0170777) == 5; }
  inline bool shape_isMyFour(int16_t s) { return (s & 0170777) == 4; }
  inline bool shape_isMyThree(int16_t s) { return (s & 0170777) == 3; }
  inline bool shape_isMyTwo(int16_t s) { return (s & 0170777) == 2; }
  inline bool shape_isOppFive(int16_t s) { return (s & 0177707) == 5*64; }
  inline bool shape_isOppFour(int16_t s) { return (s & 0177707) == 4*64; }



};
