#pragma once
#include <string>
#include <filesystem>
#include <stdio.h>
#include <fstream>

class FileManager
{
public:
  static bool saveFile(const char* data, std::string& path)
  {
      namespace fs = std::filesystem;
      /*std::string path = "/tmp";
      std::string filename;
      std::string field("filename=");
      std::string data(req.body().data());
      size_t found = data.find(field); //index of "filename="
      size_t fileSize = req.payload_size().get();
      size_t boundary = std::string(req.body().c_str()).find("\n"); //boundary length
      
      if(fileSize == 0 || boundary == 0)
      {
          printf("Error: Invalid Content-Length or boundary\n");
          return false;
      }

      //payload consists of: boundary + req.body().data() + boundary + 2
      fileSize -= data.size() + boundary + 1;
    
      if(found == std::string::npos)
      {
          printf("Error: Invalid Content-Disposition");
          return false;
      }

      //retrieve "filename=xxx"
      for(size_t i = found + field.size() + 1; i < data.size(); ++i)
          if(data[i] == '"')
              break;
          else
              filename += data[i];

      if(filename.empty())
      {
          printf("Error: could not find %s\n", filename);
          return false;
      }
      
      try
      {
          if(!fs::exists(path))
          {
              printf("Error: could not save %s in /tmp\n", filename);
              return false;
          }

          path += "/" + filename;

          if(fs::exists(path))
              fs::remove(path);
      }
      catch(const std::exception& e)
      {
          printf("Error: %s", e.what());
          return false;
      }

      std::ofstream file;

      try
      {
          const char* begin = req.body().c_str() + data.size() - 4; //begin of pure payload
          file.open(path, std::ios::out | std::ios::binary | std::ios::app);
          file.write(begin, fileSize);
          file.close();
      }
      catch(const std::exception& e)
      {
          printf("Error: save file %s", e.what());
          return false;
      }*/
      return true;
  }
};