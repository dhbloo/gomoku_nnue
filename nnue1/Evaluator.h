#pragma once
#include "global.h"
#include "EvaluatorOneSide.h"
class Evaluator
{
public:
  EvaluatorOneSide* blackEvaluator;
  EvaluatorOneSide* whiteEvaluator;
  Key               zobrist[2][BS * BS];
  Key               key;



  Evaluator(std::string type, std::string filepath);
  ~Evaluator() {delete blackEvaluator; delete whiteEvaluator;}

  void initZobrist(uint64_t seed);
  bool loadParam(std::string filepathB, std::string filepathW);
  void clear() {blackEvaluator->clear();whiteEvaluator->clear();}
  void recalculate()
  {
    blackEvaluator->recalculate();
    whiteEvaluator->recalculate();
  }
  ValueType evaluateFull(Color color, PolicyType *policy)
  {
    if (color == C_BLACK)
      return blackEvaluator->evaluateFull(policy);
    else
      return whiteEvaluator->evaluateFull(policy);
  }
  void evaluatePolicy(Color color, PolicyType *policy)
  {
    if (color == C_BLACK)
      blackEvaluator->evaluatePolicy(policy);
    else
      whiteEvaluator->evaluatePolicy(policy);
  }
  ValueType evaluateValue(Color color)
  {
    if (color == C_BLACK)
      return blackEvaluator->evaluateValue();
    else
      return whiteEvaluator->evaluateValue();
  }
  void play(Color color, Loc loc) { 
      key ^= zobrist[color - C_BLACK][loc];
      blackEvaluator->play(color, loc); 
      whiteEvaluator->play(getOpp(color), loc); 
  }
  void undo(Loc loc)
  {
    Color color = blackEvaluator->board[loc];
      key ^= zobrist[color - C_BLACK][loc];
      blackEvaluator->undo(loc); 
      whiteEvaluator->undo(loc); 
  }
  Color* board() const { return blackEvaluator->board; }

private:

  //ÿ�ε���play����undoʱ���Ȳ���EvaluatorOneSide�����ߣ���Ϊ�����ܴ��Ȼ��档
  //MCTS��ʱ�򣬾������߻�ͷ·����ʹ��cache���Դ�����١�
  struct MoveCache
  {
    bool isUndo;
    Color color;
    Loc loc;
    MoveCache() :isUndo(false), color(C_EMPTY), loc(LOC_NULL){}
    MoveCache(bool isUndo,Color color,Loc loc) :isUndo(isUndo), color(color), loc(loc){}
  };

  MoveCache moveCacheB[BS * BS], moveCacheW[BS * BS];
  int moveCacheBlength, moveCacheWlength;
};

