package util.logging;

import java.io.PrintWriter;
import java.io.IOException;
import java.text.MessageFormat;
import java.util.Date;

public class FileLogger implements Logger {
  private PrintWriter writer;

  public FileLogger() {
    try {
      writer = new PrintWriter("log.csv");
      writer.println("LogType,Time,StackTrace,Message");
      writer.flush();
    } catch (IOException e) {
      System.out.println("Could not create FileLogger");
      writer = null;
    }
  }

  @Override
  public void close() {
    if (writer != null) {
      writer.close();
    }
  }

  @Override
  public void log(Logger.LoggingType logType, Date time, String stackTrace, String message) {
    if (writer != null) {
      writer.println(MessageFormat.format("{0},{1, time},{2},{3}", logType, time, stackTrace, message));
      writer.flush();
    }
  }
}
