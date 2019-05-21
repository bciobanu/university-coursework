package flux;

public abstract class Component extends BaseComponent {
  @Override
  protected abstract StoreReduce store();
}