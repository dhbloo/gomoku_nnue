// nnue1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "AllEvaluator.h"
#include "AllSearch.h"
#include "Engine.h"
#include "EngineDev.h"
#include "TT.h"

#include <chrono>
#include <iostream>

using namespace std;

std::string appPath(int argc, const char **argv)
{
  std::string path(argv[0]);
  while (path[path.length() - 1] != '/' && path[path.length() - 1] != '\\'
         && path.length() > 0)
    path.pop_back();
  return path;
}

int maingtp(int argc, const char **argv)
{
  std::string modelPath = appPath(argc, argv) + "model.txt";
  // 如果有第二个参数，认为是model path
  if (argc > 1) {
    modelPath = argv[1];
  }

  Engine engine("mix6", modelPath, 0,false);
  engine.protocolLoop();
  return 0;
}

int main_testsearch()
{
  Evaluator *eva    = new Evaluator("mix6", "weights/t1e2.txt");
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
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . o o . . . . . . . "
                          ". . . . . . o . o x . . . . . "
                          ". . . . . . . x . . . . . . . "
                          ". . . . . . . . x . . . . . . "
                          ". . . . . . . . . x . . . . . "
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

int main1_play()  // play a game
{
  Evaluator *eva = new Evaluator("mix6", "weights/t1e2.txt");
  // Evaluator *eva    = new Evaluator("sum1", "weights/sum1.txt");
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
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . o x o o . . . . . "
                          ". . . . . . . x . . . . . . . "
                          ". . . . . . . . x x . . . . . "
                          ". . . . . . . . . . . . . . . "
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

  Color enginecolor = C_WHITE;
  while (1) {
    Time   tic = now();
    Loc    bestloc;
    string valuetext;
    for (int depth = 0; depth < 100; depth++) {
      Loc    loc;
      double value = search->fullsearch(enginecolor, depth, loc);
      Time   toc   = now();
      // search->evaluator->recalculate();
      cout << "Depth = " << depth << " Value = " << valueText(value)
           << " Bestloc = " << loc % BS << "," << loc / BS << " Nodes = " << search->nodes
           << "(" << search->interiorNodes << ")"
           << " Time = " << toc - tic << " Nps = " << search->nodes * 1000.0 / (toc - tic)
           << endl;
      if (toc - tic > 5000) {
        bestloc   = loc;
        valuetext = valueText(value);
        break;
      }
    }

    eva->play(enginecolor, bestloc);

    for (int y = 0; y < BS; y++) {
      for (int x = 0; x < BS; x++) {
        Color c = eva->board()[y * BS + x];
        if (y * BS + x == bestloc)
          cout << "@ ";
        else if (c == C_EMPTY)
          cout << ". ";
        else if (c == C_MY)
          cout << "x ";
        else if (c == C_OPP)
          cout << "o ";
      }
      cout << endl;
    }

    cout << "BestLoc:" << char('A' + bestloc % BS) << 15 - bestloc / BS
         << "   Value:" << valuetext << endl;

    string nextmove;
    while (1) {
      cin >> nextmove;
      int x = -1, y = -1;
      if (nextmove.size() == 2) {
        x = nextmove[0] - 'a';
        y = 15 - (nextmove[1] - '0');
      }
      else if (nextmove.size() == 3) {
        x = nextmove[0] - 'a';
        y = 15 - 10 * (nextmove[1] - '0') - (nextmove[2] - '0');
      }
      if (x >= 0 && x < BS && y >= 0 && y < BS) {
        eva->play(getOpp(enginecolor), MakeLoc(x, y));
        break;
      }
      else
        cout << "Bad input" << endl;
    }
  }
  return 0;
}

int main_testABsearch()
{
  Evaluator *eva    = new Evaluator("mix6", "weights/t1e.txt");
  Search *   search = new ABsearch(eva);
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
                          ". . . . . . . . . . . . . . . "
                          ". . . . . . o . . . . . . . . "
                          ". . . . . . o . o . . . . . . "
                          ". . . . . . . x . . . . . . . "
                          ". . . . . . . . x . . . . . . "
                          ". . . . . . . . . x . . . . . "
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
    cout << "Depth = " << depth << " Value = " << value << "Loc = " << loc
         << " Time = " << toc - tic << endl;
  }
  return 0;
}

int main_testeval()
{
  Eva_sum1 *eva = new Eva_sum1();
  eva->loadParam("weights/sum1.txt");
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
                          ". . . . . . o . . . . . . . . "
                          ". . . . . . o . . . . . . . . "
                          ". . . . . . o . o o . . . . . "
                          ". . . . . . . x . . . . . . . "
                          ". . . . . . . . x . . . . . . "
                          ". . . . . . . . . x . . . . . "
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

  eva->debug_print();
  return 0;
}

int main(int argc, const char **argv)
{
  return maingtp(argc, argv);
  // return main_testeval();
}