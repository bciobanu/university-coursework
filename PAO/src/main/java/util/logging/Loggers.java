package util.logging;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Loggers implements Logger {
  private static Loggers instance = new Loggers();
  private List<Logger> loggers = new ArrayList<>();

  private Loggers() {
  }

  public static Loggers get() {
    return instance;
  }

  public void addLogger(Logger logger) {
    loggers.add(logger);
  }

  @Override
  public void close() {
    for (Logger l : loggers) {
      l.close();
    }
  }

  @Override
  public void log(LoggingType logType, Date time, String stackTrace, String message) {
    for (Logger l : loggers) {
      l.log(logType, time, stackTrace, message);
    }
  }
}
