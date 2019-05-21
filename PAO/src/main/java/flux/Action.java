package flux;

import java.util.*;

public class Action implements java.io.Serializable {
  private int componentId;
  private Object actionType;
  private List<Object> payload;

  public Action(int componentId, Object actionType, Object... payload) {
    this.componentId = componentId;
    this.actionType = actionType;
    this.payload = Arrays.asList(payload);
  }

  public int getComponentId() {
    return componentId;
  }

  public Object getActionType() {
    return actionType;
  }

  public List<Object> getPayload() {
    return payload;
  }
}
