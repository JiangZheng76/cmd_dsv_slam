#pragma once
#include "FullSystem/HessianBlocks.h"
#include "cmd_frontend.hpp"
#include "util/FrameShell.h"

using namespace cmd;

class DSVComm : public FrontEndComm {
 public:
  DSVComm(std::string ip, std::string port);
  virtual ~DSVComm() {}

  // 需要重写的函数
  void publishLoopframe(dso::FrameHessian *fh, dso::CalibHessian *HCalib,
                        precision_t dso_error = 0.01,
                        precision_t scale_error = 0.01);
private:
    int cur_id_ = -1;
};