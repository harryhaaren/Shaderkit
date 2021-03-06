#include "parser/glsl_pp.hpp"
#include "core/log.hpp"

#include <QFile>
#include <QStringList>

#include <fcntl.h>
#include <unistd.h>

using namespace Shaderkit;

void macroChildren(const GLSLpp::MacroValue& v, std::string ind = "  ")
{
  for (auto it = v.children.begin(); it != v.children.end(); ++it) {
    Log::info("%s'%s' src:'%s' partial:'%s', value:'%s'", ind.c_str(),
              it->name.c_str(), it->src.c_str(), it->partially_expanded.c_str(), it->value.c_str());
    macroChildren(*it, ind + "  ");
  }
}

int main(int argc, char* argv[])
{
  Log log;

  if (argc == 3 && std::string(argv[1]) == "--debug") {
    QFile file(argv[2]);
    if (file.open(QFile::ReadOnly)) {
      GLSLpp p;
      QByteArray in = file.readAll();
      p.parse(in);
      Log::info("Version: %d '%s'", p.version(), p.profile().c_str());
      auto& lv = p.lineValues();
      auto lines_in = in.split('\n');
      QStringList lines = QString(p.out().c_str()).split("\n");
      int len = 0;
      int lines_count = lines.size();
      if (lines.last().isEmpty()) --lines_count;
      for (int l = 0; l < lines_count; ++l)
        len = std::max(len, lines[l].size());
      for (int l = 0; l < lines_count; ++l)
        Log::info("%s %d\t%*s | %s", lv[l] ? "true " : "false", l+1, -len, lines[l].toUtf8().data(), lines_in[l].data());
      auto& e = p.extensions();
      for (auto it = e.begin(); it != e.end(); ++it)
        Log::info("Extension: '%s' - %d", it->first.c_str(), it->second);
      auto& pr = p.pragmas();
      for (auto it = pr.begin(); it != pr.end(); ++it)
        Log::info("Pragma: '%s' - %d", it->first.c_str(), it->second);
      /*auto& m = p.possibleUnknownMacros();
      for (auto it = m.begin(); it != m.end(); ++it)
        Log::info("Might be a macro: '%s'", it->c_str());*/
      auto& ma = p.macros();
      for (auto it = ma.begin(); it != ma.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
          const GLSLpp::MacroValue& v = *it2;
          Log::info("Macro '%s' src:'%s' partial:'%s', value:'%s' (line %d)", v.name.c_str(),
                    v.src.c_str(), v.partially_expanded.c_str(), v.value.c_str(), it->first);
          macroChildren(v);
        }
      }
      return 0;
    } else {
      Log::error("Failed to open %s", argv[2]);
      return 1;
    }
  }

  if (argc != 2) {
    Log::error("Usage: %s <file>", argv[0]);
    return 1;
  }

  int out = dup(1);
  close(1);
  int n = open("/dev/null", O_WRONLY);

  QFile file(argv[1]);
  if (file.open(QFile::ReadOnly)) {
    GLSLpp p;
    p.parse(file.readAll());
    write(out, p.out().c_str(), p.out().size());
  } else {
    Log::error("Failed to open %s", argv[1]);
  }

  close(out);
  close(n);
}
