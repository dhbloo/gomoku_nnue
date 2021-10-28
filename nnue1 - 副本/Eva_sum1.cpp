#include "Eva_sum1.h"

const double Eva_sum1::quantFactor=32;//��������=32

bool Eva_sum1::loadParam(std::ifstream& fs)
{
  Total_Eval_Num = 0;
  for (int i = 0; i < shapeNum; i++)
  {
    for (int j = 0; j < 4; j++)weight[j][i]=Weight_No_Implement;
  }
  int id;
  while (fs >> id)
  {
    //std::cout << "a";
    for (int j = 0; j < 4; j++)fs>>weight[j][id];
    //std::cout << id << " "<< weight[0][id]<<std::endl;
  }
  //�Ѿ����ӵĵط���policy��ΪMIN_POLICY
  for (int i = 0; i < shapeNum; i++)//
  {
    if(((i/81)%3!=0)&& weight[0][i]!=Weight_No_Implement)//�����ҺϷ�������
       weight[0][i]=MIN_POLICY;

  }
  clear();
  return true;
}

void Eva_sum1::clear()
{
  for (int i = 0; i < BS * BS; i++)board[i] = C_EMPTY;
  initShapeTable();

  winSum = 0, lossSum = 0, drawSum = 0;
  for (int i = 0; i < BS * BS; i++)policyBuf[i] = 0;
  for (int dir = 0; dir < 4; dir++)
  {
    for (int loc = 0; loc < BS * BS; loc++)
    {
      //std::cout << winSum << " " << lossSum << std::endl;
      int shape = shapeTable[dir][loc];
      policyBuf[loc] += weight[0][shape];
      winSum += weight[1][shape];
      lossSum += weight[2][shape];
      drawSum += weight[3][shape];
    }
  }
}

void Eva_sum1::recalculate()
{
  Color boardCopy[BS * BS];
  memcpy(boardCopy, board, BS * BS * sizeof(Color));
  clear();
  for (int i = 0; i < BS * BS; i++)
  {
    if (boardCopy[i] != C_EMPTY)play(boardCopy[i], i);
  }
}

void Eva_sum1::play(Color color, Loc loc)
{
  board[loc] = color;
  auto affectOnOnePoint = [&](int loc, int dir, int dist)//dist����0��������С��0�Ǹ�����
  {
    int oldShape = shapeTable[dir][loc];
    int newShape = oldShape + color * pow3[dist + 4];
    shapeTable[dir][loc] = newShape;
    //if (weight[0][newShape] == Weight_No_Implement)std::cout << "Weight No implement "<<newShape<<std::endl;//debug
    policyBuf[loc] = policyBuf[loc] - weight[0][oldShape] + weight[0][newShape];
    winSum = winSum - weight[1][oldShape] + weight[1][newShape];
    lossSum = lossSum - weight[2][oldShape] + weight[2][newShape];
    drawSum = drawSum - weight[3][oldShape] + weight[3][newShape];
  };
  int x0 = loc % BS;
  int y0 = loc / BS;

  int dxs[4] = { 1,0,1,1 };
  int dys[4] = { 0,1,1,-1 };

  for (int dir = 0; dir < 4; dir++)
  {
    for (int dist = -4; dist <= 4; dist++)
    {
      int x = x0 - dist * dxs[dir];
      int y = y0 - dist * dys[dir];
      if (x < 0 || x >= BS || y < 0 || y >= BS)continue;
      affectOnOnePoint(x + BS * y, dir, dist);
    }
  }
}

double Eva_sum1::evaluate(PolicyType* policy)
{
  Total_Eval_Num++;
  //if (Total_Eval_Num % 100000 == 0)std::cout << "TotalEval " << Total_Eval_Num << std::endl;
  if (policy != nullptr)
  {
    memcpy(policy, policyBuf, sizeof(policyBuf));
  }
  return getWinlossRate();
}

void Eva_sum1::undo(Loc loc)
{
  Color color = board[loc];
  board[loc] = C_EMPTY;
  auto affectOnOnePoint = [&](int loc, int dir, int dist)//dist����0��������С��0�Ǹ�����
  {
    int oldShape = shapeTable[dir][loc];
    int newShape = oldShape - color * pow3[dist + 4];
    shapeTable[dir][loc] = newShape;
    policyBuf[loc] = policyBuf[loc] - weight[0][oldShape] + weight[0][newShape];
    winSum = winSum - weight[1][oldShape] + weight[1][newShape];
    lossSum = lossSum - weight[2][oldShape] + weight[2][newShape];
    drawSum = drawSum - weight[3][oldShape] + weight[3][newShape];
  };
  int x0 = loc % BS;
  int y0 = loc / BS;

  int dxs[4] = { 1,0,1,1 };
  int dys[4] = { 0,1,1,-1 };

  for (int dir = 0; dir < 4; dir++)
  {
    for (int dist = -4; dist <= 4; dist++)
    {
      int x = x0 - dist * dxs[dir];
      int y = y0 - dist * dys[dir];
      if (x < 0 || x >= BS || y < 0 || y >= BS)continue;
      affectOnOnePoint(x + BS * y, dir, dist);
    }
  }
}

void Eva_sum1::initShapeTable()
{
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < BS * BS; j++)
      shapeTable[i][j] = 0;

  //���²��ɽ���������Ϊ���ڸ���
  
  //�������ǽ����ǽ����ǽ������ǽ������ǽ��
  
  //���1 
  for (int i = 0; i < BS; i++)
  {
    int c = 19683;//3����1000000000
    shapeTable[0][(BS - 4) + i * BS] = c;//��ǽ
    shapeTable[1][i + (BS - 4) * BS] = c;//��ǽ
    shapeTable[2][(BS - 4) + i * BS] = c;//����ǽ����
    shapeTable[2][i + (BS - 4) * BS] = c;//����ǽ����
    shapeTable[3][(BS - 4) + i * BS] = c;//����ǽ����
    shapeTable[3][i + 3 * BS] = c;//����ǽ����
  }
  //���2 
  for (int i = 0; i < BS; i++)
  {
    int c = 26244;//3����1100000000
    shapeTable[0][(BS - 3) + i * BS] = c;//��ǽ
    shapeTable[1][i + (BS - 3) * BS] = c;//��ǽ
    shapeTable[2][(BS - 3) + i * BS] = c;//����ǽ����
    shapeTable[2][i + (BS - 3) * BS] = c;//����ǽ����
    shapeTable[3][(BS - 3) + i * BS] = c;//����ǽ����
    shapeTable[3][i + 2 * BS] = c;//����ǽ����
  }
  //���3 
  for (int i = 0; i < BS; i++)
  {
    int c = 28431;//3����1110000000
    shapeTable[0][(BS - 2) + i * BS] = c;//��ǽ
    shapeTable[1][i + (BS - 2) * BS] = c;//��ǽ
    shapeTable[2][(BS - 2) + i * BS] = c;//����ǽ����
    shapeTable[2][i + (BS - 2) * BS] = c;//����ǽ����
    shapeTable[3][(BS - 2) + i * BS] = c;//����ǽ����
    shapeTable[3][i + 1 * BS] = c;//����ǽ����
  }
  //���4 
  for (int i = 0; i < BS; i++)
  {
    int c = 29160;//3����1111000000
    shapeTable[0][(BS - 1) + i * BS] = c;//��ǽ
    shapeTable[1][i + (BS - 1) * BS] = c;//��ǽ
    shapeTable[2][(BS - 1) + i * BS] = c;//����ǽ����
    shapeTable[2][i + (BS - 1) * BS] = c;//����ǽ����
    shapeTable[3][(BS - 1) + i * BS] = c;//����ǽ����
    shapeTable[3][i + 0 * BS] = c;//����ǽ����
  }



  //�������ǽ����ǽ����ǽ������ǽ������ǽ��

  //���1 
  for (int i = 0; i < BS; i++)
  {
    int c = 39366;//3����2000000000
    shapeTable[0][3 + i * BS] = c;//��ǽ
    shapeTable[1][i + 3 * BS] = c;//��ǽ
    shapeTable[2][3 + i * BS] = c;//����ǽ����
    shapeTable[2][i + 3 * BS] = c;//����ǽ����
    shapeTable[3][3 + i * BS] = c;//����ǽ����
    shapeTable[3][i + (BS - 4) * BS] = c;//����ǽ����
  }
  //���2 
  for (int i = 0; i < BS; i++)
  {
    int c = 39367;//3����2000000001
    shapeTable[0][2 + i * BS] = c;//��ǽ
    shapeTable[1][i + 2 * BS] = c;//��ǽ
    shapeTable[2][2 + i * BS] = c;//����ǽ����
    shapeTable[2][i + 2 * BS] = c;//����ǽ����
    shapeTable[3][2 + i * BS] = c;//����ǽ����
    shapeTable[3][i + (BS - 3) * BS] = c;//����ǽ����
  }
  //���3 
  for (int i = 0; i < BS; i++)
  {
    int c = 39370;//3����2000000011
    shapeTable[0][1 + i * BS] = c;//��ǽ
    shapeTable[1][i + 1 * BS] = c;//��ǽ
    shapeTable[2][1 + i * BS] = c;//����ǽ����
    shapeTable[2][i + 1 * BS] = c;//����ǽ����
    shapeTable[3][1 + i * BS] = c;//����ǽ����
    shapeTable[3][i + (BS - 2) * BS] = c;//����ǽ����
  }
  //���4
  for (int i = 0; i < BS; i++)
  {
    int c = 39379;//3����2000000111
    shapeTable[0][0 + i * BS] = c;//��ǽ
    shapeTable[1][i + 0 * BS] = c;//��ǽ
    shapeTable[2][0 + i * BS] = c;//����ǽ����
    shapeTable[2][i + 0 * BS] = c;//����ǽ����
    shapeTable[3][0 + i * BS] = c;//����ǽ����
    shapeTable[3][i + (BS - 1) * BS] = c;//����ǽ����
  }


  //���߶���ǽ

  for (int a = 1; a <= 4; a++)//������ǽ��
    for (int b = 1; b <= 4; b++)//������ǽ��
    {
      int c = 3 * pow3[9];
      for (int i = 0; i < a - 1; i++)c += pow3[8 - i];
      for (int i = 0; i < b - 1; i++)c += pow3[i];
      shapeTable[2][(BS - 5 + a) + (4 - b) * BS] = c;//���Ͻ�
      shapeTable[2][(BS - 5 + a) * BS + (4 - b)] = c;//���½�
      shapeTable[3][(4 - b) + (4 - a) * BS] = c;//���Ͻ�
      shapeTable[3][(BS-5+ a) + (BS - 5 + b) * BS] = c;//���½�

    }
}

double Eva_sum1::getWinlossRate()
{
  //softmax
  double maxValue = winSum > lossSum ? winSum : lossSum;
  maxValue = maxValue > drawSum ? maxValue : drawSum;
  double factor = BS * BS * quantFactor;
  double win = exp((winSum - maxValue) / factor);
  double loss = exp((lossSum - maxValue) / factor);
  double draw = exp((drawSum - maxValue) / factor);
  return (win-loss)/(win+loss+draw);
}
using namespace std;
void Eva_sum1::debug_print()
{
  for (int y = 0; y < BS; y++)
  {
    for (int x = 0; x < BS; x++)
    {
      Color c = board[y * BS + x];
      if (c == C_EMPTY)cout << ". ";
      else if (c == C_MY)cout << "x ";
      else if (c == C_OPP)cout << "o ";
    }
    cout << endl;
  }



  double maxValue = winSum > lossSum ? winSum : lossSum;
  maxValue = maxValue > drawSum ? maxValue : drawSum;
  double factor = BS * BS * quantFactor;
  double win = exp((winSum - maxValue) / factor);
  double loss = exp((lossSum - maxValue) / factor);
  double draw = exp((drawSum - maxValue) / factor);
  double sum = win + loss + draw;
  cout << "WR " << (win+0.5*draw) / sum << endl;
  cout << "Win " << win / sum << endl;
  cout << "Loss " << loss/sum << endl;
  cout << "Draw " << draw/sum<< endl;

  for (int y = 0; y < BS; y++)
  {
    for (int x = 0; x < BS; x++)
    {
      Color c = board[y * BS + x];
      if (c == C_EMPTY)cout << ".\t";
      else if (c == C_MY)cout << "x\t";
      else if (c == C_OPP)cout << "o\t";
    }
    cout << endl;
    for (int x = 0; x < BS; x++)
    {
      int p = policyBuf[y * BS + x];
      cout << p << "\t";
    }
    cout << endl;
  }




  /*
  auto print3 = [&](int a)//10λ������
  {
    int buf[10];
    for (int i = 0; i < 9; i++)
    {
      buf[9 - i] = a % 3;
      a = a / 3;
    }
    buf[0] = a;
    for (int i = 0; i < 10; i++)std::cout << buf[i];
    std::cout << " ";
  };
  
  for (int dir = 0; dir < 4; dir++)
  {
    std::cout << "dir=" << dir << std::endl;
    for (int y = 0; y < BS; y++)
    {
      for (int x = 0; x < BS; x++)
      {
        print3(shapeTable[dir][x + y * BS]);
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  */
}
