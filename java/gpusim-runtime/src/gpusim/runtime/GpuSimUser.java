package gpusim.runtime;

import gridsim.GridSim;
import gridsim.Gridlet;
import gridsim.GridletList;
import java.util.LinkedList;

public class GpuSimUser extends GridSim {
    public static final String GPUSIM_USER_NAME = "GpuSimUser";

    private int _gpuSimUserID;
    private LinkedList<Integer> _resourcesIDs;
    private GridletList _gridlets;

    public GpuSimUser(double linkBaudRate, GridletList gridlets) throws Exception {
        super(GPUSIM_USER_NAME, linkBaudRate);
        
        _gpuSimUserID = GridSim.getEntityId(GPUSIM_USER_NAME);
        _gridlets = gridlets;
        for(Gridlet gridlet: _gridlets) {
            gridlet.setUserID(_gpuSimUserID);
        }
    }

    public int getGpuSimUserID() {
        return _gpuSimUserID;
    }

    public LinkedList<Integer> getResourcesIDs() throws Exception {
        if (_resourcesIDs == null) {
            throw new Exception("Resources aren't avaliable");
        }

        return _resourcesIDs;
    }

    private void loadResourcesIDs() {
        LinkedList resList;
        _resourcesIDs = new LinkedList<Integer>();

        // waiting to get list of resources. Since GridSim package uses multi-threaded environment, your request might
        // arrive earlier before one or more grid resource entities manage to register themselves to
        // GridInformationService (GIS) entity. Therefore, it's better to wait in the first place
        //
        while (true) {
            // need to pause for a while to wait GridResources finish registering to GIS. Hold by 1 second.
            //
            super.gridSimHold(1.0);

            resList = GridSim.getGridResourceList();
            if (resList.size() == 0) {
                System.out.println("GpuSimUser: Waiting to get list of resources ...");
                continue;
            }

            for (Object val: resList) {
                _resourcesIDs.add((Integer)val);
            }
            break;
        }
    }


    @Override
    public void body() {
        loadResourcesIDs();
        int count = _gridlets.size();
        int resourceID = _resourcesIDs.get(0);
        for (int i = 0; i < count; ++i)
        {
            Gridlet gridlet = (Gridlet)_gridlets.get(i);
            super.gridletSubmit(gridlet, resourceID);
        }
        for (int i = 0; i < count; ++i){
            GridSimRunTime.getInstance().getOutput().addGridlet(super.gridletReceive());
        }
        super.shutdownGridStatisticsEntity();
        super.shutdownUserEntity();
        super.terminateIOEntities();

        GridSimRunTime.getInstance().getOutput().setTotalSimulationTime(GridSim.clock());
    }
}