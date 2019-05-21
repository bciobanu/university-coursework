package flux;

import util.sql.Table;

public abstract class StoreReduce extends Store {
  private Table table;

  protected StoreReduce() {
    table = createTable();
  }

  protected abstract Table createTable();

  protected abstract Object reduce(Table table, Action a);

  Table getTable() {
    return table;
  }

  @Override
  Object onDispatch(Action a) {
    return reduce(table, a);
  }
}
