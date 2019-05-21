package util.sql;

import util.Pair;
import util.logging.Loggers;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class Table {
  private String tableName;
  private List<Pair<String, DataType>> fields;

  public enum DataType {
    INT {
      @Override
      public String toString() {
        return "INT";
      }
    },
    VARCHAR {
      @Override
      public String toString() {
        return "VARCHAR(30)";
      }
    },
    DATE {
      @Override
      public String toString() {
        return "TIMESTAMP";
      }
    },
  }

  private static void runCommand(String command) throws SQLException {
    Statement stmt = ConnectionManager.get().createStatement();
    stmt.execute(command);
  }

  public static ResultSet query(String query) throws SQLException {
    Statement stmt = ConnectionManager.get().createStatement();
    return stmt.executeQuery(query);
  }

  public static class Builder {
    private String tableName;
    private List<Pair<String, DataType>> fields = new ArrayList<>();
    private List<Pair<String, String>> foreignKeys = new ArrayList<>();

    public Builder(String tableName, String primaryKey) {
      this.tableName = tableName;
      fields.add(new Pair<>(primaryKey, DataType.INT));
    }

    public Builder addField(String fieldName, DataType fieldType) {
      fields.add(new Pair<>(fieldName, fieldType));
      return this;
    }

    public Builder addForeignKey(String keyName, String otherTable) {
      fields.add(new Pair<>(keyName, DataType.INT));
      foreignKeys.add(new Pair<>(keyName, otherTable));
      return this;
    }

    public Table create() {
      StringBuilder sb = new StringBuilder();
      sb.append("CREATE TABLE IF NOT EXISTS " + tableName
          + "(" + fields.get(0).fst + " INT NOT NULL AUTO_INCREMENT");
      for (Pair<String, DataType> field : fields.subList(1, fields.size())) {
        sb.append(", " + field.fst + " " + field.snd);
      }
      for (Pair<String, String> fk : foreignKeys) {
        sb.append(", FOREIGN KEY (" + fk.fst + ") REFERENCES " + fk.snd + "(" + fk.fst + ")");
      }
      sb.append(", PRIMARY KEY (" + fields.get(0).fst + "))");
      try {
        runCommand(sb.toString());
      } catch (SQLException e) {
        Loggers.get().critical("Could not create table {0}!", tableName);
        System.exit(-1);
      }
      return new Table(tableName, fields);
    }
  }

  private Table(String tableName, List<Pair<String, DataType>> fields) {
    this.tableName = tableName;
    this.fields = fields;
  }

  public void update(int id, String field, String newValue) {
    String command = "UPDATE " + tableName
        + " SET " + field + " = " + newValue
        + " WHERE " + fields.get(0).fst + " = " + id;
    try {
      runCommand(command);
    } catch (SQLException e) {
      Loggers.get().warn("Could not run update");
    }
  }

  public void delete(int id) {
    try {
      runCommand("DELETE FROM " + tableName + " WHERE " + fields.get(0).fst + " = " + id);
    } catch (SQLException e) {
      Loggers.get().warn("Could not delete {0}", id);
    }
  }

  public int insert(Object... args) {
    if (args.length != fields.size()) {
      return -1;
    }

    StringBuilder command = new StringBuilder();
    command.append("INSERT INTO " + tableName + " VALUES(");
    for (int i = 0; i < args.length; ++i) {
      if (i != 0) {
        command.append(", ");
      }
      if (args[i] == null) {
        command.append("NULL");
      } else {
        switch (fields.get(i).snd) {
          case INT:
            command.append((int) args[i]);
            break;
          case VARCHAR:
            command.append((String) args[i]);
            break;
          case DATE:
            command.append(args[i]);
            break;
          default:
            return -1;
        }
      }
    }
    command.append(")");

    try {
      runCommand(command.toString());
    } catch (SQLException e) {
      Loggers.get().warn("Could not insert into {0}", tableName);
      return -1;
    }

    try {
      ResultSet rs = query("SELECT LAST_INSERT_ID()");
      if (rs.next()) {
        return rs.getInt("LAST_INSERT_ID()");
      }
      rs.close();
    } catch (SQLException e) {
      Loggers.get().warn("Could not get last id in {0}", tableName);
    }
    return -1;
  }
}
