package util.sql;

import util.logging.Loggers;

import java.sql.*;

public class ConnectionManager implements AutoCloseable {
  private static ConnectionManager instance = new ConnectionManager();
  private static Connection con;

  private ConnectionManager() {
    try {
      Class.forName(ConnectionConstants.JDBC_DRIVER);
      try {
        con = DriverManager.getConnection(
            ConnectionConstants.DB_URL, ConnectionConstants.USER, ConnectionConstants.PASS);
      } catch (SQLException ex) {
        Loggers.get().critical("Failed to create the database connection!");
      }
    } catch (ClassNotFoundException ex) {
      Loggers.get().critical("JDBC driver not found!");
    }
  }

  public static ConnectionManager getInstance() {
    return instance;
  }

  public static Connection get() {
    return instance.con;
  }

  @Override
  public void close() {
    try {
      if (con != null) {
        con.close();
      }
    } catch (SQLException ex) {
      Loggers.get().warn("Could not close database connection!");
    }
  }
}
