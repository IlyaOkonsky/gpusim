package gpusim.config;

import java.io.Serializable;

public class GridSimGridletConfig implements Serializable {
    //<editor-fold defaultstate="collapsed" desc="Internal fields">
    private int _id;
    private double _length;
    private long _inputSize;
    private long _outputSize;
    private long _count;
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="Constructors">
    public GridSimGridletConfig() {
        _id = 0;
        _length = 1;
        _inputSize = 1;
        _outputSize = 1;
        _count = 1;
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
    
    //<editor-fold defaultstate="collapsed" desc="Length property">
    public double getLength() {
        return _length;
    }
    
    public void setLength(double length) {
        if (length < 0) {
            throw new AssertionError(length >= 0);
        }
        
        _length = length;
    }
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="InputSize property">
    public long getInputSize() {
        return _inputSize;
    }
    
    public void setInputSize(long inputSize) {
        if (inputSize < 1) {
            throw new AssertionError(inputSize >= 1);
        }
        
        _inputSize = inputSize;
    }
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="OutputSize property">
    public long getOutputSize() {
        return _outputSize;
    }
    
    public void setOutputSize(long outputSize) {
        if (outputSize < 1) {
            throw new AssertionError(outputSize >= 1);
        }
        
        _outputSize = outputSize;
    }
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="Count property">
    public long getCount() {
        return _count;
    }
    
    public void setCount(long count) {
        if (count < 1) {
            throw new AssertionError(count >= 1);
        }
        
        _count = count;
    }
    //</editor-fold>    
    //</editor-fold>
}