package flux;

import util.logging.Loggers;

import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

public class FluxDispatcher implements Dispatcher, AutoCloseable {
  private static FluxDispatcher instance = new FluxDispatcher();
  private ExecutorService workers = Executors.newFixedThreadPool(2);
  private Map<Integer, BaseComponent> components = new HashMap<>();

  private FluxDispatcher() {
  }

  public static FluxDispatcher getInstance() {
    return instance;
  }

  public int register(BaseComponent s) {
    int id = components.size();
    components.put(id, s);
    return id;
  }

  public void unregister(int id) {
    components.remove(id);
  }

  public Optional<BaseComponent> getComponent(int id) {
    return Optional.ofNullable(components.get(id));
  }

  public Object blockingDispatch(Action o) {
    try {
      return dispatch(o).get();
    } catch (Exception e) {
      System.out.println("Dispatching error");
      return null;
    }
  }

  private Future<Object> dispatch(Action o) {
    return getComponent(o.getComponentId()).map(a -> workers.submit(() -> a.store().invokeOnDispatch(o)))
        .get();
  }

  public void close() {
    workers.shutdown();
    try {
      workers.awaitTermination(1, TimeUnit.NANOSECONDS);
    } catch (InterruptedException e) {
      Loggers.get().critical("Failed to stop workers");
    }
  }
}
