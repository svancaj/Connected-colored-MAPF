package ozomorph.app;

import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.scene.control.Label;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import ozomorph.nodes.Group;

import java.util.*;

/**
 * Controller for GUI element that shows
 * numbers of positions assigned to each group in initial and target configuration.
 */
public class DifferenciesTableController {
    private GridPane gridPane;
    private MapController initials;
    private MapController targets;
    private SetGroup setGroup;

    private List<ColumnConstraints> innerCols;

    /**
     * Indicates if there are groups with difierent number of positions in intial and target configuration.
     */
    public BooleanProperty areDifferenciesProperty;

    /**
     * Creates new DifferenciesTableController.
     * @param gridPane GridPane where the table will be shown.
     * @param initials Initial configuration.
     * @param targets Target configuration.
     * @param setGroup Delegate to request change of selected group.
     */
    public DifferenciesTableController(GridPane gridPane, MapController initials, MapController targets, SetGroup setGroup) {
        this.gridPane = gridPane;
        this.initials = initials;
        this.targets = targets;
        this.setGroup = setGroup;

        initials.addListener(e->updateTable());
        targets.addListener(e->updateTable());

        areDifferenciesProperty = new SimpleBooleanProperty(true);

        ColumnConstraints col1 = new ColumnConstraints();
        col1.setPercentWidth(33);
        ColumnConstraints col2 = new ColumnConstraints();
        col2.setPercentWidth(34);
        ColumnConstraints col3 = new ColumnConstraints();
        col3.setPercentWidth(33);
        innerCols = Arrays.asList(col1, col2, col3);

        ColumnConstraints outerCol = new ColumnConstraints();
        outerCol.setPercentWidth(100);
        gridPane.getColumnConstraints().setAll(outerCol);
        gridPane.setHgap(5);

        updateTable();
    }

    /**
     * Updates table according to current configurations.
     */
    public void updateTable(){
        gridPane.getChildren().clear();
        Set<Group> usedGroups = new HashSet<>(initials.groups.keySet());
        usedGroups.addAll(targets.groups.keySet());

        boolean areDifferencies = false;
        int agentsCount = 0;
        int row = 1;
        for (Group group : usedGroups) {
            var initialsNumber = initials.groups.getOrDefault(group, new HashSet<>()).size();
            var targetsNumber = targets.groups.getOrDefault(group, new HashSet<>()).size();
            agentsCount += targetsNumber + initialsNumber;

            if(targetsNumber == 0 && initialsNumber == 0)
                continue;

            var r = new Rectangle(10,10);
            r.setFill(group.getColor());
            r.setStroke(Color.BLACK);
            r.setStrokeWidth(2);

            GridPane item = new GridPane();
            item.addRow(0,new Label(String.valueOf(initialsNumber)),r,new Label(String.valueOf(targetsNumber)));
            item.setOnMouseClicked(e -> setGroup.setGroup(group));
            if(targetsNumber != initialsNumber){
                item.setBackground(new Background(new BackgroundFill(Color.RED.brighter(),null,null)));
                areDifferencies = true;
            }
            item.getColumnConstraints().addAll(innerCols);

            gridPane.addRow(row, item);
            row++;
        }
        if(agentsCount > 0)
            areDifferenciesProperty.setValue(areDifferencies);
        else
            areDifferenciesProperty.setValue(true);
    }
}
