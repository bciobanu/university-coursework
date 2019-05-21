import container.AppContainer;
import data.TaskComponent;
import util.logging.ConsoleLogger;
import util.logging.FileLogger;
import util.logging.Logger;
import util.logging.Loggers;

import javax.swing.*;
import javax.swing.border.MatteBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.sql.Date;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.stream.Collectors;

public class Main {
  static class FixedSizeFrame extends JFrame {
    FixedSizeFrame(int width, int height) {
      setLayout(null);
      setSize(width, height);
      setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
      setResizable(false);
      setVisible(true);
    }
  }

  public static class UserFrame extends FixedSizeFrame {
    private static JPanel tasks = new JPanel(new GridBagLayout());

    private static void addTask(int agendaHandle, int taskHandle, boolean repaint) {
      GridBagConstraints gbc = new GridBagConstraints();
      gbc.gridwidth = GridBagConstraints.REMAINDER;
      gbc.weightx = 1;
      gbc.weighty = 1;
      gbc.fill = GridBagConstraints.HORIZONTAL;
      tasks.add(new TaskPanel(taskHandle), gbc, 0);
      if (repaint) {
        tasks.validate();
        tasks.repaint();
      }
    }

    static class TaskPanel extends JPanel {
      private int taskHandle;

      class NameLabel extends JLabel {
        NameLabel(String value) {
          setText(String.format("Name: %s", value));
        }
      }

      class DescriptionLabel extends JLabel {
        DescriptionLabel(String value) {
          setText(String.format("Description: %s", value));
        }
      }

      class DateLabel extends JLabel {
        DateLabel(Date date) {
          setText(String.format("Overdue: %s", date.toString()));
        }
      }

      class DeleteButton extends JButton {
        DeleteButton(TaskPanel toDelete) {
          setText("Delete");
          addActionListener((ActionEvent e) -> {
            AppContainer.getInstance().completeTask(taskHandle);
            tasks.remove(toDelete);
            tasks.revalidate();
            tasks.repaint();
          });
        }
      }

      TaskPanel(int taskHandle) {
        this.taskHandle = taskHandle;
        TaskComponent.TaskView taskView = new TaskComponent().view();

        int numItems = 1;
        if (taskView.getName(taskHandle) != null) {
          add(new NameLabel(taskView.getName(taskHandle)));
          ++numItems;
        }

        if (taskView.getDescription(taskHandle) != null) {
          add(new DescriptionLabel(taskView.getDescription(taskHandle)));
          ++numItems;
        }

        if (taskView.getEndTime(taskHandle) != null) {
          add(new DateLabel(taskView.getEndTime(taskHandle)));
          ++numItems;
        }

        add(new DeleteButton(this));
        setLayout(new GridLayout(numItems, 1));
        setBorder(new MatteBorder(0, 0, 1, 0, Color.GRAY));
      }
    }

    static class AddFrame extends FixedSizeFrame {
      private class NameLabel extends JLabel {
        NameLabel() {
          setText("Name:");
        }
      }

      private static class NameTextField extends JTextField {
        private static final AddFrame.NameTextField instance = new AddFrame.NameTextField();

        private NameTextField() {
          setColumns(8);
        }

        static AddFrame.NameTextField get() {
          return instance;
        }
      }

      private class DescriptionLabel extends JLabel {
        DescriptionLabel() {
          setText("Description: ");
        }
      }

      private static class DescriptionTextField extends JTextField {
        private static final AddFrame.DescriptionTextField instance = new AddFrame.DescriptionTextField();

        private DescriptionTextField() {
          setColumns(8);
        }

        static AddFrame.DescriptionTextField get() {
          return instance;
        }
      }

      private class DateLabel extends JLabel {
        DateLabel() {
          setText("Date: ");
        }
      }

      private static class DateTextField extends JTextField {
        private static final AddFrame.DateTextField instance = new AddFrame.DateTextField();

        private DateTextField() {
          setText("dd-MM-yyyy");
          setColumns(8);
        }

        static AddFrame.DateTextField get() {
          return instance;
        }
      }

      private class SubmitButton extends JButton {
        private Date parseDate(String date) {
          SimpleDateFormat sdf = new SimpleDateFormat("dd-MM-yyyy");
          try {
            return new Date(sdf.parse(date).getTime());
          } catch (ParseException parseException) {
            Loggers.get().warn("Invalid date format");
          }
          return null;
        }

        SubmitButton(int agendaHandle) {
          setText("Submit");
          addActionListener((ActionEvent e) -> {
            AppContainer app = AppContainer.getInstance();
            String name = AddFrame.NameTextField.get().getText();
            String description = String.valueOf(AddFrame.DescriptionTextField.get().getText());
            Date date = parseDate(AddFrame.DateTextField.get().getText());
            if (name.length() == 0
                && description.length() == 0
                && date == null) {
              return;
            }

            int taskHandle = app.createTask(agendaHandle);
            if (name.length() != 0) {
              app.setTaskName(taskHandle, name);
            }
            if (description.length() != 0) {
              app.setTaskDescription(taskHandle, description);
            }
            if (date != null) {
              app.setTaskEndTime(taskHandle, date);
            }
            addTask(agendaHandle, taskHandle, true);
          });
        }
      }

      AddFrame(int agendaHandle, JButton disableButton) {
        super(200, 150);
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();

        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        c.gridy = 0;
        add(new NameLabel(), c);

        c.gridx = 1;
        add(NameTextField.get(), c);

        c.gridy = 1;
        c.gridx = 0;
        add(new DescriptionLabel(), c);

        c.gridx = 1;
        add(DescriptionTextField.get(), c);

        c.gridy = 2;
        c.gridx = 0;
        add(new DateLabel(), c);

        c.gridx = 1;
        add(DateTextField.get(), c);

        c.gridy = 3;
        c.gridwidth = 2;
        c.gridx = 0;
        add(new SubmitButton(agendaHandle), c);

        addWindowListener(new WindowAdapter() {
          public void windowClosing(WindowEvent e) {
            disableButton.setEnabled(true);
          }
        });
      }
    }

    class AddButton extends JButton {
      AddButton(int agendaHandle) {
        setText("Add");
        addActionListener((ActionEvent e) -> {
          setEnabled(false);
          new AddFrame(agendaHandle, this);
        });
      }
    }

    UserFrame(int userHandle) {
      super(500, 500);
      AppContainer app = AppContainer.getInstance();
      int agendaHandle = app.createAgenda(userHandle);

      GridBagConstraints gbc = new GridBagConstraints();
      gbc.gridwidth = GridBagConstraints.REMAINDER;
      gbc.weightx = 1;
      gbc.weighty = 1;
      gbc.fill = GridBagConstraints.HORIZONTAL;
      tasks.add(new JPanel(), gbc);
      for (Integer taskHandle
          : new TaskComponent().view().getTasks(agendaHandle).collect(Collectors.toList())) {
        addTask(agendaHandle, taskHandle, false);
      }
      setLayout(new BorderLayout());
      add(new JScrollPane(tasks), BorderLayout.CENTER);
      add(new AddButton(agendaHandle), BorderLayout.SOUTH);
    }
  }

  public static class LoginFrame extends FixedSizeFrame {
    private class UserNameLabel extends JLabel {
      UserNameLabel() {
        setText("Username:");
      }
    }

    private static class UserNameTextField extends JTextField {
      private static final UserNameTextField instance = new UserNameTextField();

      private UserNameTextField() {
        setColumns(8);
      }

      static UserNameTextField get() {
        return instance;
      }
    }

    private class PasswordLabel extends JLabel {
      PasswordLabel() {
        setText("Password: ");
      }
    }

    private static class PasswordTextField extends JPasswordField {
      private static final PasswordTextField instance = new PasswordTextField();

      private PasswordTextField() {
        setColumns(8);
      }

      static PasswordTextField get() {
        return instance;
      }
    }

    private class SubmitButton extends JButton {
      SubmitButton() {
        setText("Submit");
        addActionListener((ActionEvent e) -> {
          AppContainer app = AppContainer.getInstance();
          String username = UserNameTextField.get().getText();
          String password = String.valueOf(PasswordTextField.get().getPassword());
          app.registerUser(username, password);

          int handle = app.login(username, password);
          if (handle == -1) {
            handle = app.registerUser(username, password);
            if (handle == -1) {
              JOptionPane.showMessageDialog(this, "Could not login");
              return;
            }
          }
          dispose();
          new UserFrame(handle);
        });
      }
    }

    LoginFrame() {
      super(200, 100);
      setLayout(new GridBagLayout());
      GridBagConstraints c = new GridBagConstraints();

      c.fill = GridBagConstraints.HORIZONTAL;
      c.gridx = 0;
      c.gridy = 0;
      add(new UserNameLabel(), c);

      c.gridx = 1;
      add(UserNameTextField.get(), c);

      c.gridy = 1;
      c.gridx = 0;
      add(new PasswordLabel(), c);

      c.gridx = 1;
      add(PasswordTextField.get(), c);

      c.gridy = 2;
      c.gridwidth = 2;
      c.gridx = 0;
      add(new SubmitButton(), c);
    }
  }

  public static class StartupFrame extends FixedSizeFrame {
    private class LoginButton extends JButton {
      LoginButton() {
        setText("Login");
        setBounds(10, 10, 200, 30);
        addActionListener((ActionEvent e) -> {
          dispose();
          new LoginFrame();
        });
      }
    }

    StartupFrame() {
      super(220, 90);
      setTitle("Jorge");
      add(new LoginButton());
    }
  }

  public static void main(String[] args) {
    // register loggers
    Loggers logger = Loggers.get();
    logger.addLogger(new ConsoleLogger());
    logger.addLogger(new FileLogger());

    Runtime.getRuntime().addShutdownHook(new Thread(() -> {
      System.out.println("Exiting..");
      AppContainer.getInstance().close();
      logger.close();
    }));
    new StartupFrame();
  }
}
