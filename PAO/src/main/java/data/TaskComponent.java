package data;

import flux.StoreReduce;
import flux.View;
import flux.ViewableComponent;
import util.sql.Table;
import util.sql.Table.DataType;

import java.sql.Date;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.stream.Stream;

public class TaskComponent extends ViewableComponent {
  public enum Action {
    CREATE_TASK,
    COMPLETE_TASK,
    SET_NAME,
    SET_DESCRIPTION,
    SET_ENDTIME
  }

  public class TaskStore extends StoreReduce {
    @Override
    protected Table createTable() {
      return new Table.Builder("TASKS", "TaskID")
          .addForeignKey("AgendaID", "AGENDAS")
          .addField("Name", DataType.VARCHAR)
          .addField("Description", DataType.VARCHAR)
          .addField("Date", DataType.DATE)
          .create();
    }

    @Override
    protected Object reduce(Table table, flux.Action a) {
      switch ((Action)a.getActionType()) {
        case CREATE_TASK:
          return table.insert(null, a.getPayload().get(0), null, null, null);
        case COMPLETE_TASK:
          table.delete((int) a.getPayload().get(0));
          return -1;
        case SET_NAME:
          table.update((int) a.getPayload().get(0), "Name", "'" + a.getPayload().get(1) + "'");
          return -1;
        case SET_DESCRIPTION:
          table.update((int) a.getPayload().get(0), "Description", "'" + a.getPayload().get(1) + "'");
          return -1;
        case SET_ENDTIME:
          table.update((int) a.getPayload().get(0), "Date", "'" + a.getPayload().get(1) + "'");
          return -1;
        default:
          return -1;
      }
    }
  }

  public class TaskView extends View {
    TaskView() {
    }

    public Stream<Integer> getTasks(int agendaHandle) {
      try {
        ResultSet rs = Table.query("SELECT TaskID FROM TASKS WHERE AgendaID = " + agendaHandle);
        Stream.Builder<Integer> result = Stream.builder();
        while (rs.next()) {
          result.add(rs.getInt("TaskID"));
        }
        rs.close();
        return result.build();
      } catch (SQLException e) {
        return Stream.empty();
      }
    }

    public String getName(int taskHandle) {
      String result = null;
      try {
        ResultSet rs = Table.query("SELECT Name FROM TASKS WHERE TaskID = " + taskHandle);
        if (rs.next()) {
          result = rs.getString("Name");
        }
        rs.close();
      } catch (SQLException e) {
      }
      return result;
    }

    public String getDescription(int taskHandle) {
      String result = null;
      try {
        ResultSet rs = Table.query("SELECT Description FROM TASKS WHERE TaskID = " + taskHandle);
        if (rs.next()) {
          result = rs.getString("Description");
        }
        rs.close();
      } catch (SQLException e) {
      }
      return result;
    }

    public Date getEndTime(int taskHandle) {
      Date result = null;
      try {
        ResultSet rs = Table.query("SELECT Date FROM TASKS WHERE TaskID = " + taskHandle);
        if (rs.next()) {
          result = rs.getDate("Date");
        }
        rs.close();
      } catch (SQLException e) {
      }
      return result;
    }
  }

  private TaskStore store = new TaskStore();

  @Override
  protected TaskStore store() {
    return store;
  }

  @Override
  public TaskView view() {
    return new TaskView();
  }
}
