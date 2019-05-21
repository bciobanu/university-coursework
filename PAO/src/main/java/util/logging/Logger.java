package util.logging;

import java.util.Date;
import java.text.MessageFormat;

public interface Logger {
  enum LoggingType {
    INFO {
      @Override
      public final String toString() {
        return "INFO";
      }
    },
    WARNING {
      @Override
      public String toString() {
        return "WARNING";
      }
    },
    CRITICAL {
      @Override
      public String toString() {
        return "CRITICAL";
      }
    },
    DEBUG {
      @Override
      public String toString() {
        return "DEBUG";
      }
    },
  }

  void log(LoggingType logType, Date time, String stackTrace, String message);

  private void bindLog(LoggingType logType, String format, Object... args) {
    log(logType,
        new Date(System.currentTimeMillis()),
        Thread.currentThread().getStackTrace()[3].toString(),
        MessageFormat.format(format, args));
  }

  default void close() {}

  default void info(String format, Object... args) {
    bindLog(LoggingType.INFO, format, args);
  }

  default void warn(String format, Object... args) {
    bindLog(LoggingType.WARNING, format, args);
  }

  default void critical(String format, Object... args) {
    bindLog(LoggingType.CRITICAL, format, args);
  }

  default void debug(String format, Object... args) {
    bindLog(LoggingType.DEBUG, format, args);
  }
}
