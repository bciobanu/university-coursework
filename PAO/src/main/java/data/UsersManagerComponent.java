package data;

import flux.Component;
import flux.StoreReduce;
import flux.View;
import util.sql.Table;

import java.sql.ResultSet;
import java.sql.SQLException;

public class UsersManagerComponent extends Component {
  public enum Action {
    CREATE_USER,
    CHANGE_NAME,
    CHANGE_PASSWORD
  }

  @Override
  protected UsersManagerStore store() {
    return store;
  }

  private UsersManagerStore store = new UsersManagerStore();

  public class UsersManagerStore extends StoreReduce {
    @Override
    protected Table createTable() {
      return new Table.Builder("USERS", "UserID")
          .addField("Name", Table.DataType.VARCHAR)
          .addField("Password", Table.DataType.VARCHAR)
          .create();
    }

    @Override
    protected Object reduce(Table table, flux.Action a) {
      if (a.getActionType() == Action.CREATE_USER) {
        return table.insert(null, null, null);
      } else if (a.getActionType() == Action.CHANGE_NAME) {
        table.update((int) a.getPayload().get(0),
            "Name", "'" + a.getPayload().get(1) + "'");
      } else if (a.getActionType() == Action.CHANGE_PASSWORD) {
        table.update((int) a.getPayload().get(0),
            "Password", "'" + a.getPayload().get(1) + "'");
      }
      return null;
    }
  }

  public class UsersManagerView extends View {
    UsersManagerView() {
    }

    public int login(String user, String password) {
      int result = -1;
      try {
        ResultSet rs = Table.query(
            "SELECT UserID FROM USERS WHERE Name = '" + user + "' AND Password = '" + password + "'");
        if (rs.next()) {
          result = rs.getInt("UserID");
        }
      } catch (SQLException e) {
      }
      return result;
    }
  }

  public UsersManagerView view() {
    return new UsersManagerView();
  }
}
