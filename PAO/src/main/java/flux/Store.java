package flux;

abstract class Store {
  Object invokeOnDispatch(Action a) {
    return onDispatch(a);
  }

  abstract Object onDispatch(Action a);
}
