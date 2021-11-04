// nnue1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "AllSearch.h"
#include "Engine.h"
#include "TT.h"

#include <chrono>
#include <iostream>

typedef int64_t Time;  // value in milliseconds

Time now()
{
  static_assert(sizeof(Time) == sizeof(std::chrono::milliseconds::rep),
                "Time should be 64 bits");

  auto dur = std::chrono::steady_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}

using namespace std;
int main()
{
  Evaluator *eva    = new Evaluator("mix6", "weights/t5.txt");
  PVSsearch *search = new PVSsearch(eva);
  TT.resize(128);
  /*
  const char boardstr[] = ""
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . "
    ". . . . . . . . . . . . . . . ";
    */

  const char boardstr[] = ""
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . x . . . . . . . "
                          ". . . . . . . o . . . . . . . "
                          ". . . . . . o x o o x . . . . "
                          ". . . . . . . x x x o . . . . "
                          ". . . . . . . o x x . . . . . "
                          ". . . . . . . o . . o . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . ";
  for (int y = 0; y < BS; y++)
    for (int x = 0; x < BS; x++) {
      char  colorchar = boardstr[2 * (x + y * BS)];
      Color color     = C_EMPTY;
      if (colorchar == 'x')
        color = C_BLACK;
      else if (colorchar == 'o')
        color = C_WHITE;
      if (color != C_EMPTY)
        eva->play(color, MakeLoc(x, y));
    }

  Time tic = now();
  for (int depth = 0; depth < 100; depth++) {
    Loc    loc;
    double value = search->fullsearch(C_BLACK, depth, loc);
    Time   toc   = now();
    // search->evaluator->recalculate();
    cout << "Depth = " << depth << " Value = " << valueText(value)
         << " Nodes = " << search->nodes << "(" << search->interiorNodes << ")"
         << " Time = " << toc - tic << " Nps = " << search->nodes * 1000.0 / (toc - tic)
         << " TT = " << 100.0 * search->ttHits / search->interiorNodes << "("
         << 100.0 * search->ttCuts / search->ttHits << ")"
         << " PV = " << search->rootPV() << endl;
  }
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5.
//   转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
