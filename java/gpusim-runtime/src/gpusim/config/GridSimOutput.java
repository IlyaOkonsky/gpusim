package gpusim.config;

import gridsim.Gridlet;
import java.io.Serializable;
import java.util.LinkedList;

public class GridSimOutput implements Serializable {
    //<editor-fold defaultstate="collapsed" desc="Constants">
    /**
     * Current target version of the GridSimOutput entity.
     */
    public static final int CURRENT_OUTPUT_VERSION = 1;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Internal fields">
    private int _version;
    private String _name;
    private double _totalSimulationTime;
    private LinkedList<GridSimGridletOutput> _gridlets;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Constructors">
    public GridSimOutput() {
        _version = CURRENT_OUTPUT_VERSION;
        _name = "Unnamed GridSim Output";
        _totalSimulationTime = 0.0f;
        _gridlets = new LinkedList<GridSimGridletOutput>();
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Properties">
    //<editor-fold defaultstate="collapsed" desc="Version property">
    public int getVersion() {
        return _version;
    }

    public void setVersion(int version) {
        if (version <= 0) {
            throw new AssertionError(version > 0);
        }

        _version = version;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Name property">
    public String getName() {
        return _name;
    }

    public void setName(String name) {
        if (name == null) {
            throw new AssertionError(name != null);
        }

        _name = name;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="TotalSimulationTime property">
    public double getTotalSimulationTime() {
        return _totalSimulationTime;
    }

    public void setTotalSimulationTime(double totalSimulationTime) {
        if (totalSimulationTime < 0.0f) {
            throw new AssertionError(totalSimulationTime >= 0.0f);
        }

        _totalSimulationTime = totalSimulationTime;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Gridlets property">
    public LinkedList<GridSimGridletOutput> getGridlets() {
        return _gridlets;
    }

    public void setGridlets(LinkedList<GridSimGridletOutput> gridlets) {
        if (gridlets == null) {
            throw new AssertionError(gridlets != null);
        }

        _gridlets = gridlets;
    }
    //</editor-fold>
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="Methods">
    public void addGridlet(GridSimGridletOutput gridlet) {
        _gridlets.add(gridlet);
    }
    
    public void addGridlet(Gridlet gridlet) {
        GridSimGridletOutput go = new GridSimGridletOutput();
        go.setID(gridlet.getGridletID());
        go.setExecStartTime(gridlet.getExecStartTime());
        go.setExecFinishTime(gridlet.getFinishTime());
        go.setProcessingCost(gridlet.getProcessingCost());
        go.setHistory(gridlet.getGridletHistory());
        addGridlet(go);
    }
    //</editor-fold>
}