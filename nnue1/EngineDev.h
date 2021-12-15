#pragma once
//����ͬengine��ֻ�ǿ����ڲ�ɾengine�����ͬʱ���в���
#include "AllSearch.h"
#include "Evaluator.h"
#include "Search.h"
#include "global.h"

class EngineDev
{
public:
  Evaluator *   evaluator;
  MCTSsearch *      search;
  Color         nextColor;
  const bool    writeLogEnable;
  std::ofstream logfile;
  EngineDev(std::string evaluator_type, std::string weightfile, std::string configfile,bool writeLogEnable);
  EngineDev(const EngineDev &e) = delete;
  EngineDev(EngineDev &&e)      = delete;
  ~EngineDev()
  {
    delete evaluator;
    delete search;
    logfile.close();
  }

  int timeout_turn;
  int timeout_match;
  int time_left;


  static constexpr int ReservedTime          = 30;
  static constexpr int AsyncWaitReservedTime = 70;

  std::string genmove();

  void        writeLog(std::string str);

  void protocolLoop();
};
