package data;

import flux.StoreReduce;
import flux.View;
import flux.ViewableComponent;
import util.sql.Table;
import util.logging.Loggers;

import java.sql.ResultSet;
import java.sql.SQLException;

public class AgendaComponent extends ViewableComponent {
  public enum Action {
    CREATE_AGENDA
  }

  public class AgendaStore extends StoreReduce {
    @Override
    protected Table createTable() {
      return new Table.Builder("AGENDAS", "AgendaID")
          .addForeignKey("UserID", "USERS")
          .create();
    }

    @Override
    protected Object reduce(Table table, flux.Action a) {
      if (a.getActionType() == Action.CREATE_AGENDA) {
        return table.insert(null, a.getPayload().get(0));
      }
      return null;
    }
  }

  public class AgendaView extends View {
    AgendaView() {
    }

    public int getAgenda(int userHandle) {
      int result = -1;
      try {
        ResultSet rs = Table.query("SELECT AgendaID FROM AGENDAS WHERE UserID = " + userHandle);
        if (rs.next()) {
          result = rs.getInt("AgendaID");
        }
        rs.close();
      } catch (SQLException e) {
        Loggers.get().warn("Could not get agenda for user {0}!", userHandle);
      }
      return result;
    }
  }

  private AgendaStore store = new AgendaStore();

  @Override
  protected AgendaStore store() {
    return store;
  }

  @Override
  public AgendaView view() {
    return new AgendaView();
  }
}
