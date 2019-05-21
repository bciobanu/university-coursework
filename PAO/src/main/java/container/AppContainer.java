package container;

import data.AgendaComponent;
import data.TaskComponent;
import data.UsersManagerComponent;
import flux.Action;
import flux.Dispatcher;
import flux.FluxDispatcher;
import util.sql.ConnectionManager;

import java.sql.Date;

public class AppContainer implements AutoCloseable {
  private static final AppContainer instance = new AppContainer();
  private final Dispatcher dispatcher = FluxDispatcher.getInstance();

  // register services
  private final int USERS_MANAGER = new UsersManagerComponent().id();
  private final int AGENDAS_MANAGER = new AgendaComponent().id();
  private final int TASKS_MANAGER = new TaskComponent().id();

  private AppContainer() {
  }

  public static AppContainer getInstance() {
    return instance;
  }

  public int registerUser(String name, String password) {
    int id = (int) dispatcher.blockingDispatch(
        new Action(USERS_MANAGER, UsersManagerComponent.Action.CREATE_USER));
    dispatcher.blockingDispatch(new Action(USERS_MANAGER, UsersManagerComponent.Action.CHANGE_NAME, id, name));
    dispatcher.blockingDispatch(new Action(USERS_MANAGER, UsersManagerComponent.Action.CHANGE_PASSWORD, id, password));
    return id;
  }

  public int login(String name, String password) {
    return new UsersManagerComponent().view().login(name, password);
  }

  public int getAgenda(int userHandle) {
    return new AgendaComponent().view().getAgenda(userHandle);
  }

  public int createAgenda(int userHandle) {
    int registeredAgenda = getAgenda(userHandle);
    if (registeredAgenda == -1) {
      registeredAgenda = (int) dispatcher.blockingDispatch(new Action(AGENDAS_MANAGER, AgendaComponent.Action.CREATE_AGENDA, userHandle));
    }
    return registeredAgenda;
  }

  public int createTask(int agendaHandle) {
    return (int) dispatcher.blockingDispatch(new Action(TASKS_MANAGER, TaskComponent.Action.CREATE_TASK, agendaHandle));
  }

  public void completeTask(int taskHandle) {
    dispatcher.blockingDispatch(new Action(TASKS_MANAGER, TaskComponent.Action.COMPLETE_TASK, taskHandle));
  }

  public void setTaskName(int taskHandle, String name) {
    dispatcher.blockingDispatch(new Action(TASKS_MANAGER, TaskComponent.Action.SET_NAME, taskHandle, name));
  }

  public void setTaskDescription(int taskHandle, String description) {
    dispatcher.blockingDispatch(new Action(TASKS_MANAGER, TaskComponent.Action.SET_DESCRIPTION, taskHandle, description));
  }

  public void setTaskEndTime(int taskHandle, Date date) {
    dispatcher.blockingDispatch(new Action(TASKS_MANAGER, TaskComponent.Action.SET_ENDTIME, taskHandle, date));
  }

  @Override
  public void close() {
    FluxDispatcher.getInstance().close();
    ConnectionManager.getInstance().close();
  }
}
