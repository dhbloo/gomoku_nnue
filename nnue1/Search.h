#pragma once
#include "global.h"
#include "Evaluator.h"
class Search
{
public:
  Evaluator* evaluator;//��engine���������evaluator��������������
  const Color* boardPointer;
  Search(Evaluator* e):evaluator(e),boardPointer(e->blackEvaluator->board){}
  virtual float fullsearch(Color color, double factor, Loc& bestmove) = 0;//factor����������������ָ���������ڵ����ȣ���Խ����������Խ��
  virtual void play(Color color, Loc loc) { evaluator->play(color, loc); }
  virtual void undo(Color color, Loc loc) { evaluator->undo(color, loc); }
  virtual void stop() = 0;
};
