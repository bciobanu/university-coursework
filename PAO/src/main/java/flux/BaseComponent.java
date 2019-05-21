package flux;

public abstract class BaseComponent {
  private int id = FluxDispatcher.getInstance().register(this);

  public int id() {
    return id;
  }

  protected abstract Store store();
}
