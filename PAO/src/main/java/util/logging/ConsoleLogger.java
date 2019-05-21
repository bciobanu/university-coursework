package util.logging;

import java.text.MessageFormat;
import java.util.Date;

public class ConsoleLogger implements Logger {
  static final String ANSI_RESET = "\u001B[0m";
  static final String ANSI_BLACK = "\u001B[30m";
  static final String ANSI_RED = "\u001B[31m";
  static final String ANSI_GREEN = "\u001B[32m";
  static final String ANSI_YELLOW = "\u001B[33m";
  static final String ANSI_BLUE = "\u001B[34m";
  static final String ANSI_PURPLE = "\u001B[35m";
  static final String ANSI_CYAN = "\u001B[36m";
  static final String ANSI_WHITE = "\u001B[37m";

  private static String prettyLogType(LoggingType logType) {
    String color;
    switch (logType) {
      case INFO:
        color = ANSI_BLACK;
        break;
      case WARNING:
        color = ANSI_YELLOW;
        break;
      case CRITICAL:
        color = ANSI_RED;
        break;
      case DEBUG:
        color = ANSI_PURPLE;
        break;
      default:
        color = ANSI_RESET;
    }
    return color + logType + ANSI_RESET;
  }

  @Override
  public void log(LoggingType logType, Date time, String stackTrace, String message) {
    System.out.println(MessageFormat.format(
        "{0} \t {1, time} \t {2} \t {3}", prettyLogType(logType), time, stackTrace, message));
  }
}
