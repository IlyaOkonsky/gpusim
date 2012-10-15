package gpusim.config;

import java.io.Serializable;

public class GridSimGridletOutput implements Serializable {
    //<editor-fold defaultstate="collapsed" desc="Internal fields">
    private int _id;
    private double _execStartTime;
    private double _execFinishTime;
    private double _processingCost;
    private String _history;
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Constructors">
    public GridSimGridletOutput() {
        _id = 0;
        _execStartTime = 0.0f;
        _execFinishTime = 0.0f;
        _processingCost = 0.0f;
        _history = null;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Properties">
    //<editor-fold defaultstate="collapsed" desc="ID property">
    public int getID() {
        return _id;
    }
    
    public void setID(int id) {
        if (id < 0) {
            throw new AssertionError(id >= 0);
        }
        
        _id = id;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="ExecStartTime property">
    public double getExecStartTime() {
        return _execStartTime;
    }
    
    public void setExecStartTime(double execStartTime) {
        if (execStartTime < 0.0f) {
            throw new AssertionError(execStartTime >= 0.0f);
        }
        
        _execStartTime = execStartTime;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="ExecFinishTime property">
    public double getExecFinishTime() {
        return _execFinishTime;
    }

    public void setExecFinishTime(double execFinishTime) {
        if (execFinishTime < 0.0f) {
            throw new AssertionError(execFinishTime >= 0.0f);
        }

        _execFinishTime = execFinishTime;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="ProcessingCost property">
    public double getProcessingCost() {
        return _processingCost;
    }

    public void setProcessingCost(double processingCost) {
        if (processingCost < 0.0f) {
            throw new AssertionError(processingCost >= 0.0f);
        }

        _processingCost = processingCost;
    }
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="History property">
    public String getHistory() {
        return _history;
    }

    public void setHistory(String history) {
        _history = history;
    }
    //</editor-fold>
    //</editor-fold>
}