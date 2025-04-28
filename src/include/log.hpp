#pragma once
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <filesystem>

/**
 * @brief class to save logs, default location is bin directory
 */
class Log
{
  static inline bool rwt_ = false;
  static inline std::mutex mutex_;
  std::string path_;

public:
  Log()
  {
    path_ = std::filesystem::current_path().string();
  }
/**
 * @brief get current timestamp, return string
 */
  std::string GetDateTime()
  {
    time_t timestamp = time(&timestamp);
    struct tm* dt = localtime(&timestamp);
    char buf[80];
    strftime(buf, 80, "%d/%m/%Y %T", dt);
    return std::string(buf);
  }

  /**
   * @brief set direcory to file log.txt
   */
  void SetPath(const std::string& path) { path_ = path; }

protected:
/**
 * @brief write log
 */
  void WriteLog(std::string&& str)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ofstream log;
    std::string dt = GetDateTime();
    dt.pop_back();
    std::string file_path = path_ + "/" + "log.txt";

  if(!rwt_) //erase previous session logging
  {
    log.open(file_path);
    rwt_ = true;
  }
  else    //continue logging session
    log.open(file_path, std::ios_base::app);

  if(log.is_open())
  {
    log << dt << " " << str << "\n";
    log.close();
  }
    std::cout << str << std::endl;
  }

  /**
   * @brief read log.txt file, return string
   */
  std::string GetLog()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ifstream log;
    std::string file_path = path_ + "/" + "log.txt";
    log.open(file_path);
    std::string str;
    std::string file;

    if(log.is_open())
    {
      while(std::getline(log, str))
        file += str + "\n";

      log.close();
      return file;
    }
    return str;
  }
};