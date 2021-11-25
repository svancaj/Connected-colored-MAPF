package ozomorph.app;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;


/**
 * JavaFX App
 */
public class App extends Application {

    private static Scene scene;
    private static Logger logger = LoggerFactory.getLogger(App.class);

    /**
     * "Main" method of JavaFX aplication. Shows main window.
     * @param stage Window to show.
     * @throws IOException IO error.
     */
    @Override
    public void start(Stage stage) throws IOException {
        logInfo();

        scene = new Scene(loadFXML("mainView"));
        stage.setScene(scene);
        stage.setTitle("OzoMorph");
        stage.show();
    }

    /**
     * Log useful debug info.
     */
    private void logInfo(){
        var f = new java.io.File(".");
        try {
            logger.info(".=" + f.getCanonicalPath());
        } catch (IOException e) {
            logger.error("Cannot get canonical path of .");
        }
        logger.info("user.dir=" + System.getProperty("user.dir"));
        logger.info("isMacOSX = "+ String.valueOf(isMacOSX()));
    }

    /**
     * Sets root UI element of this window.
     * @param fxml FXML file describing this window.
     * @throws IOException IO error.
     */
    static void setRoot(String fxml) throws IOException {
        scene.setRoot(loadFXML(fxml));
    }

    /**
     * Load FXML file.
     * @param fxml FXML file to load.
     * @return Root node of loaded FXML.
     * @throws IOException IO error.
     */
    private static Parent loadFXML(String fxml) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(App.class.getResource(fxml + ".fxml"));
        return fxmlLoader.load();
    }

    static{
        //setting current.date system property to use it as log-file name
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
        System.setProperty("current.date", dateFormat.format(new Date()));

        if(isMacOSX())
            System.setProperty( "jdk.lang.Process.launchMechanism", "FORK" );
    }

    /**
     * Check if app is running on MacOS.
     * @return If app is running on MacOS
     */
    private static boolean isMacOSX() {
        String os = System.getProperty("os.name").toLowerCase();
        return os.contains("mac") || os.contains("darwin");
    }

    /**
     * Application main method.
     * @param args Command line arguments.
     */
    public static void main(String[] args) {
        launch();
    }

}