package flux;

import java.util.Optional;

public interface Dispatcher {
  int register(BaseComponent component);

  void unregister(int id);

  Optional<BaseComponent> getComponent(int id);

  Object blockingDispatch(Action o);

  void close();
}
