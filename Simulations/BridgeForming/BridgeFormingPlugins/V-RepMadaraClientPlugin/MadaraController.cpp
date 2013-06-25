/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "MadaraController.h"

#include "Custom_Transport.h"
#include "CommonMadaraVariables.h"

#include <vector>

// Multicast address.
#define DEFAULT_MULTICAST_ADDRESS "239.255.0.1:4150"

// Macro to convert from int to std::string.
#define INT_TO_STR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace SMASH::Utilities;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor, sets up a Madara knowledge base and basic values.
////////////////////////////////////////////////////////////////////////////////////////////////////////
MadaraController::MadaraController(int id, double commRange)
{
    // Start the counter at 0.
    m_regionId = 0;

    // Define the transport.
    m_host = "";
    m_transportSettings.hosts_.resize (1);
    m_transportSettings.hosts_[0] = DEFAULT_MULTICAST_ADDRESS;
    //m_transportSettings.type = Madara::Transport::MULTICAST;
    m_transportSettings.delay_launch = true;
    m_transportSettings.id = id;

    // Create the knowledge base.
    m_knowledge = new Madara::Knowledge_Engine::Knowledge_Base(m_host, m_transportSettings);

    //m_knowledge->log_to_file("madaralog.txt", true);
    //m_knowledge->evaluate("#log_level(1)");

    // Add the actual transport.
    m_knowledge->attach_transport(new Custom_Transport (m_knowledge->get_id (),
        m_knowledge->get_context (), m_transportSettings, true));
   
    // Set our id and comm range.
    m_id = id;
    m_commRange = commRange;
    m_knowledge->set (MV_COMM_RANGE, m_commRange);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructor, simply cleans up.
////////////////////////////////////////////////////////////////////////////////////////////////////////
MadaraController::~MadaraController()
{
    terminate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cleanup, terminating all threads and open communications.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MadaraController::terminate()
{
    if(m_knowledge != NULL)
    {
        m_knowledge->close_transport();
        m_knowledge->clear();
        delete m_knowledge;
        m_knowledge = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets a drone as stopped.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraController::stopDrone(int droneId)
{
    // Simulate the sink actually sending the command to bridge.
    std::string droneIdString = INT_TO_STR(droneId);
    m_knowledge->set(MV_MOBILE(droneIdString), (Madara::Knowledge_Record::Integer) 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convenience method that updates both the status of this controller, as well as the status of the
// drone with the information form the simulator.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraController::updateNetworkStatus(double controllerPosx, double controllerPosy, std::vector<DroneStatus> droneStatusList)
{
    // Updates status about myself and the drones with info form the simulation.
    updateDroneStatus(droneStatusList);

    // This is done just to ensure this is propagated, since we are just setting this value to the same value it already has.
    m_knowledge->set (MV_COMM_RANGE, m_commRange, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);

    // This call has no delay to flush all past changes, and updates the total of drones in the system.
    m_knowledge->set(MV_TOTAL_DEVICES, (Madara::Knowledge_Record::Integer) droneStatusList.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Updates the status of the drones in Madara. This should be done in each drone, but since V-Rep is
// simulating them, it has to come from the controller, which is at the simulator.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraController::updateDroneStatus(std::vector<DroneStatus> droneStatusList)
{
    // Update the status of all drones.
    for (std::vector<DroneStatus>::iterator it = droneStatusList.begin() ; it != droneStatusList.end(); ++it)
    {
        // Update the overall status of this drone.
        // NOTE: we use substring below to store the information not in the local but a global variable, which is only needed in a simulation.
        std::string droneIdString = INT_TO_STR(it->id);
        m_knowledge->set((MV_DEVICE_LAT(droneIdString)).substr(1), it->posx, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
        m_knowledge->set((MV_DEVICE_LON(droneIdString)).substr(1), it->posy, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);

        //if(it->flying)
        //    m_knowledge->set(MV_MOBILE(it->id), 1.0, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
        //else
        //    m_knowledge->set(MV_MOBILE(it->id), 0.0, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    }

    //m_knowledge->print_knowledge (1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets up all the variables required for a bridge request.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraController::setupBridgeRequest(int bridgeId, Position sourceTopLeft, Position sourceBottomRight, Position sinkTopLeft, Position sinkBottomRight)
{
    int rectangleType = 0;
    std::string bridgeIdString = INT_TO_STR(bridgeId);

    // Store the id of the source region for this bridge.
    int sourceRegionId = m_regionId++;
    std::string sourceRegionIdString = INT_TO_STR(sourceRegionId);
    m_knowledge->set(MV_BRIDGE_SOURCE_REGION_ID(bridgeIdString), (Madara::Knowledge_Record::Integer) sourceRegionId, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set(MV_REGION_TYPE(sourceRegionIdString), (Madara::Knowledge_Record::Integer) rectangleType, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);

    // Set the bounding box of the regions. For now, the rectangle will actually just be a point.
    // NOTE: we use substring below to store the information not in the local but a global variable, which is only needed in a simulation.
    m_knowledge->set((MV_REGION_TOPLEFT_LAT(sourceRegionIdString)).substr(1), sourceTopLeft.x, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set((MV_REGION_TOPLEFT_LON(sourceRegionIdString)).substr(1), sourceTopLeft.y, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set((MV_REGION_BOTRIGHT_LAT(sourceRegionIdString)).substr(1), sourceBottomRight.x, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set((MV_REGION_BOTRIGHT_LON(sourceRegionIdString)).substr(1), sourceBottomRight.y, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);

    // Store the id of the sink region for this bridge.
    int sinkRegionId = m_regionId++;
    std::string sinkRegionIdString = INT_TO_STR(sinkRegionId);
    m_knowledge->set(MV_BRIDGE_SINK_REGION_ID(bridgeIdString), (Madara::Knowledge_Record::Integer) sinkRegionId, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set(MV_REGION_TYPE(sinkRegionIdString), (Madara::Knowledge_Record::Integer) rectangleType, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);

    // Set the bounding box of the regions. For now, the rectangle will actually just be a point.
    // NOTE: we use substring below to store the information not in the local but a global variable, which is only needed in a simulation.
    m_knowledge->set((MV_REGION_TOPLEFT_LAT(sinkRegionIdString)).substr(1), sinkTopLeft.x, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set((MV_REGION_TOPLEFT_LON(sinkRegionIdString)).substr(1), sinkTopLeft.y, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set((MV_REGION_BOTRIGHT_LAT(sinkRegionIdString)).substr(1), sinkBottomRight.x, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);
    m_knowledge->set((MV_REGION_BOTRIGHT_LON(sinkRegionIdString)).substr(1), sinkBottomRight.y, Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS);

    // We set the total bridges to the bridge id + 1, since it starts at 0.
    // This call has no delay to flush all past changes.
    int totalBridges = bridgeId + 1;
    m_knowledge->set(MV_TOTAL_BRIDGES, (Madara::Knowledge_Record::Integer) totalBridges);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets a target position where a drone should move to.
////////////////////////////////////////////////////////////////////////////////////////////////////////
MovementCommand* MadaraController::getNewMovementCommand(int droneId)
{
    // Apart from checking if it is busy, we will also check if the command to move was sent.
    std::string droneIdString = INT_TO_STR(droneId);
    std::string movementCommand = m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_REQUESTED).to_string();
    bool noMovementCommandSet = movementCommand.compare("0") == 0;
    if(noMovementCommandSet)
    {       
        // No position to return since there is no command yet.
        return NULL;
    }

    // Create the movement command to return.
    MovementCommand* command = new MovementCommand();
    command->command = movementCommand;

    // Depending on the command, we may need to get more parameters.
    if(movementCommand.compare(MO_MOVE_TO_GPS_CMD) == 0)
    {
        // This is a move to certain location command; get the target location.
        double targetPosX = m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_TARGET_LAT).to_double();
        double targetPosY = m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_TARGET_LON).to_double();
        Position targetPosition = Position(targetPosX, targetPosY);
        command->position = targetPosition;
    }

    // Set the command as 0 locally, to indicate that we already read it.
    m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_REQUESTED, "0", Madara::Knowledge_Engine::TREAT_AS_LOCAL_EVAL_SETTINGS);

    //m_knowledge->print_knowledge (1);

    return command;
}