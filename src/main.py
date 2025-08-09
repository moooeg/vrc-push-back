# ---------------------------------------------------------------------------- #
#                                                                              #
# 	Module:       main.py                                                      #
# 	Author:       EricGu, EthanHuang & ThomasCobiac                            # 
# 	Created:      5/12/2025, 11:58:20 AM                                       #
# 	Description:  V5 project                                                   #
#                                                                              #
#   Team 75477M 'Frij'                                                         #
#   Push Back 2025-2026                                                        #
#                                                                              #
# ---------------------------------------------------------------------------- #

# --------------------------------------
# Wiring Guide
# Updated: 2025-08-01

# Port 01 : left motor a
# Port 02 : 
# Port 03 : left motor b
# Port 04 : left motor c
# Port 05 : 
# Port 06 : left odom
# Port 07 : right odom
# Port 08 : right motor a
# Port 09 : right motor b
# Port 10 : right motor c
# Port 11 : -
# Port 12 : -
# Port 13 : -
# Port 14 : -
# Port 15 : - optical1
# Port 16 : - intake2
# Port 17 : -
# Port 18 : - intake1
# Port 19 : -
# Port 20 : -
# Port 21 : -

# Port A  : - angular
# Port B  : -
# Port C  : - trap_door
# Port D  : -
# Port E  : -
# Port F  : -
# Port G  : -
# Port H  : -
# --------------------------------------

# ---------------------------------------------------------------------------- #
# port define template
# motor = Motor(Ports.PORT1, GearSetting.RATIO_6_1, True)
# pneumatics = DigitalOut(brain.three_wire_port.a)
# optical = Optical(Ports.PORT3)
# distance = Distance(Ports.PORT4)
# ---------------------------------------------------------------------------- #

# ! Initialization
# Library imports
from vex import *

brain = Brain()
controller_1 = Controller(PRIMARY)

# ports settings 
# ! broken ports: 2
left_motor_a = Motor(Ports.PORT1, GearSetting.RATIO_6_1, True)
left_motor_b = Motor(Ports.PORT3, GearSetting.RATIO_6_1, False)
left_motor_c = Motor(Ports.PORT4, GearSetting.RATIO_6_1, True)
left_drive_smart = MotorGroup(left_motor_a,  left_motor_b, left_motor_c)

right_motor_a = Motor(Ports.PORT8, GearSetting.RATIO_6_1, True)
right_motor_b = Motor(Ports.PORT9, GearSetting.RATIO_6_1, False)
right_motor_c = Motor(Ports.PORT10, GearSetting.RATIO_6_1, False)
right_drive_smart = MotorGroup(right_motor_a, right_motor_b, right_motor_c)

drivetrain = DriveTrain(left_drive_smart, right_drive_smart, 299.24, 377.1, 304.8, MM)

intake1 = Motor(Ports.PORT17, GearSetting.RATIO_6_1, False)
intake2 = Motor(Ports.PORT16, GearSetting.RATIO_6_1, False)


left_odom =  Rotation(Ports.PORT6, False)
right_odom = Rotation(Ports.PORT7, True)

optical = Optical(Ports.PORT18)

angular = DigitalOut(brain.three_wire_port.a) #true: High goal, false: Low goal
trap_door = DigitalOut(brain.three_wire_port.c) #true: Open, false: close


# ! GUI setup
# -Side Selection GUI
class ButtonPosition:
    def __init__(self, x1: int, y1: int, x2: int, y2: int) -> None:
        """
        x1, y1: top left

        x2, y2: bottom right
        """
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def pressing(self, x: int, y: int) -> bool:
        """
        Return true if the passed x and y coordinate is inside this button
        """
        return (self.x1 <= x <= self.x2) and (self.y1 <= y <= self.y2)

GUI_BUTTONS_POSITIONS = {
    "top": {
        "1": ButtonPosition(139, 8, 240, 26),
        "2": ButtonPosition(249, 8, 351, 26),
        "3": ButtonPosition(358, 8, 461, 26)
    },
    "side": {
        "1": ButtonPosition(19, 52, 138, 73),
        "2": ButtonPosition(19, 85, 138, 107),
        "3": ButtonPosition(19, 120, 138, 142)
    }
}

class TeamPosition:
    def __init__(self, team: str = "", position: str = ""):
        self.team = team
        self.position = position
    def __str__(self) -> str:
        return self.team + "_" + self.position

# -side selection
def team_choosing(is_skill: bool = False) -> TeamPosition:
    '''
    Start choosing team, can use controller button to select or use GUI on brain.

    Args:
        isSkill (bool): If True, directly select "skill"
    '''
    # SS GUI init
    brain.screen.draw_image_from_file("begin.png", 0, 0)

    team_position = TeamPosition()
    confirmed = False

    if is_skill:
        team_position.team = "skill"
        brain.screen.draw_image_from_file("skill_confirmed.png", 0, 0)
        return team_position

    while True:
        wait(5, MSEC)

        # exit
        if confirmed:
            brain.screen.draw_image_from_file(str(team_position) + "_confirmed.png", 0, 0)
            return team_position

        # controller
        if controller_1.buttonL1.pressing():
            team_position.team = "red"
            team_position.position = "1"
            confirmed = True
        elif controller_1.buttonL2.pressing():
            team_position.team = "red"
            team_position.position = "2"
            confirmed = True
        elif controller_1.buttonR1.pressing():
            team_position.team = "blue"
            team_position.position = "1"
            confirmed = True
        elif controller_1.buttonR2.pressing():
            team_position.team = "blue"
            team_position.position = "2"
            confirmed = True
        elif controller_1.buttonA.pressing():
            team_position.team = "skill"
            team_position.position = ""
            confirmed = True

        # brain
        if brain.screen.pressing():
            x = brain.screen.x_position()
            y = brain.screen.y_position()

            if GUI_BUTTONS_POSITIONS["top"]["1"].pressing(x, y):
                team_position.team = "red"
                team_position.position = ""
                brain.screen.draw_image_from_file("red_begin.png", 0, 0)
            elif GUI_BUTTONS_POSITIONS["top"]["2"].pressing(x, y):
                team_position.team = "blue"
                team_position.position = ""
                brain.screen.draw_image_from_file("blue_begin.png", 0, 0)
            elif GUI_BUTTONS_POSITIONS["top"]["3"].pressing(x, y):
                team_position.team = "skill"
                team_position.position = ""
                brain.screen.draw_image_from_file("skill_begin.png", 0, 0)

            if team_position.team:
                if GUI_BUTTONS_POSITIONS["side"]["1"].pressing(x, y):
                    if team_position.team == "skill":
                        team_position.position = ""
                        confirmed = True
                    else:
                        team_position.position = "1"
                        brain.screen.draw_image_from_file(team_position.team+"_1.png", 0, 0)
                elif GUI_BUTTONS_POSITIONS["side"]["2"].pressing(x, y) and team_position.team != "skill":
                    team_position.position = "2"
                    brain.screen.draw_image_from_file(team_position.team + "_2.png", 0, 0)
                elif GUI_BUTTONS_POSITIONS["side"]["3"].pressing(x, y) and team_position.position and team_position.team != "skill":
                    confirmed = True
            
            wait_until_release(brain.screen.pressing, 50)

# ! All functions
# -misc.
def cprint(_input: Any) -> None:
    '''
    Print to the controller screen, erase previous content
    Args:
        _input (Any): any type of input
    '''
    s = str(_input)
    controller_1.screen.clear_screen()
    controller_1.screen.set_cursor(1,1)
    controller_1.screen.print(s)

def wait_until_release(fn, time: float) -> None:
    '''
    Block until the passed function return False

    Args:
        fn (function): must returm bool
        time: time to wait in ms
    '''
    while fn():
        wait(time, MSEC)

def clamp(number: int | float, minimum: int | float, maximum: int | float) -> int | float:
    '''
    Clamp number into given range

    Examples:
        clamp(8, 0, 10) >>> 8

        clamp(20, 0, 10) >>> 10

        clamp(-20, -10, 10) >>> -10
    '''
    return max(min(number, maximum), minimum)

def color_sort(team_pos: TeamPosition): #color to remain 
    '''
    Sorting Opponent color blocks
    
    Args:
        team_pos: Team Position include current team color
    '''
    print("hi")
    while True:
        if team_pos.team == "blue":
            if 0 < optical.hue() < 40: # red hue value
                trap_door.set(True)
                wait(500, MSEC)
            else:
                trap_door.set(False)
        elif team_pos.team == "red":
            if 150 < optical.hue() < 230: # blue hue value
                trap_door.set(True)
                wait(500, MSEC)
            else:
                trap_door.set(False)
        else:
            trap_door.set(False)
        wait(20, MSEC)

# -thread in driver control
def drivetrain_control():
    '''
    Control the drivetrain using the controller
    '''
    # Variables initialisation
    left_drive_smart_stopped = False
    right_drive_smart_stopped = False
    left_drive_smart_speed = 0
    right_drive_smart_speed = 0
    integral_rotate = 0
    
    while True:
        ratio = 1.5  # Bigger the number, less sensitive
        integral_decay_rate = 0.000009  # Rate at which integral decays
        forward = 100 * math.sin((controller_1.axis3.position()**3) / 636620)
        rotate_dynamic = (100 / ratio) * math.sin(abs(forward**3) / 636620) * math.sin(controller_1.axis1.position()**3 / 636620)
        rotate_linear = 40 * math.sin(controller_1.axis1.position()**3 / 636620)
        max_integral_limit = 0.6*rotate_dynamic
        
        # Accumulate integral when joystick is pushed
        if abs(controller_1.axis1.position()) >= 35:
            integral_rotate += rotate_dynamic * integral_decay_rate
            if integral_rotate > 0:
                integral_rotate = min(integral_rotate, max_integral_limit)  # Cap the integral value
            elif integral_rotate < 0:
                integral_rotate = max(integral_rotate, max_integral_limit)
        else:
            integral_rotate = 0  # Reset the integral when joystick is back below 30

        # Add integral component to turning calculation
        if -20 <= forward <= 20:
            left_drive_smart_speed =  forward + rotate_linear
            right_drive_smart_speed = forward - rotate_linear
        else:
            # Use the integral component
            left_drive_smart_speed =  forward + rotate_dynamic - integral_rotate
            right_drive_smart_speed = forward - rotate_dynamic + integral_rotate

        if abs(left_drive_smart_speed) < 1:
            if left_drive_smart_stopped:
                left_drive_smart.stop()
                left_drive_smart_stopped = False
        else:
            left_drive_smart_stopped = True

        if abs(right_drive_smart_speed) < 1:
            if right_drive_smart_stopped:
                right_drive_smart.stop()
                right_drive_smart_stopped = False
        else:
            right_drive_smart_stopped = True

        if left_drive_smart_stopped:
            left_drive_smart.set_velocity(left_drive_smart_speed, PERCENT)
            left_drive_smart.spin(FORWARD)

        if right_drive_smart_stopped:
            right_drive_smart.set_velocity(right_drive_smart_speed, PERCENT)
            right_drive_smart.spin(FORWARD)
            
def controller_intake():    
    while True:
        if controller_1.buttonR1.pressing():
            intake1.spin(FORWARD)
            intake2.spin(FORWARD)
            wait_until_release(controller_1.buttonR1.pressing, 5)
        elif controller_1.buttonR2.pressing():
            intake1.spin(REVERSE)
            intake2.spin(REVERSE)
            wait_until_release(controller_1.buttonR2.pressing, 5)
        else:
            intake1.stop()
            intake2.stop()
        wait(20, MSEC)
        
def scoring_angle():
    '''
    Set the scoring angle using the controller
    '''
    while True:
        if controller_1.axis2.position() > 80:
            angular.set(True)  # Toggle the angular status
            wait_until_release(lambda: controller_1.axis2.position() > 80, 20)
        elif controller_1.axis2.position() < -80:
            angular.set(False)  # Toggle the angular status
            wait_until_release(lambda: controller_1.axis2.position() < -80, 20)

        wait(20, MSEC)

# -autonomous functions
def drivetrain_forward(left_target_turns: float, right_target_turns: float, chain_status = False, speed=100, time_out=0):
    '''
    Move forward using PID control
    Args:
        left_target_turns (float): target turns for left motor
        right_target_turns (float): target turns for right motor
        chain_status (bool): True if not the last motion for motion chain, default is False
        speed (int): speed of the motors, default is 100
        time_out (int): time out in ms, default is 0, 0 means no time out
    '''
    movement_start_time = brain.timer.time(MSEC)
    false_condition_start_time = None
    
    kp = 28
    ki = 0.001
    kd = 100
    
    left_err = 0
    right_err = 0
    
    left_integral = 0
    right_integral = 0
    
    left_derivitive = 0
    right_derivitive = 0
    
    left_prev_error = 0
    right_prev_error = 0
    
    init_left_odom = left_odom.position(TURNS)
    init_right_odom = right_odom.position(TURNS)
    
    current_left_odom = left_odom.position(TURNS)
    current_right_odom = right_odom.position(TURNS)
    
    left_drive_smart.spin(FORWARD)
    right_drive_smart.spin(FORWARD)
    
    while True:
        left_err = left_target_turns - (current_left_odom - init_left_odom)
        right_err = right_target_turns - (current_right_odom - init_right_odom)
        
        left_integral = (left_integral + left_err)*0.99
        right_integral = (right_integral + right_err)*0.99
        
        left_derivitive = left_err - left_prev_error
        right_derivitive = right_err - right_prev_error
        
        left_speed = (speed/100)*(max(min((kp * left_err) + (ki * left_integral) + (kd * left_derivitive), 100), -100))
        right_speed = (speed/100)*(max(min((kp * right_err) + (ki * right_integral) + (kd * right_derivitive), 100), -100))
        
        left_prev_error = left_err
        right_prev_error = right_err
        
        left_drive_smart.set_velocity(left_speed, PERCENT)
        right_drive_smart.set_velocity(right_speed, PERCENT)
        
        current_left_odom = left_odom.position(TURNS)
        current_right_odom = right_odom.position(TURNS)
        
        if not chain_status:
            if not ((left_target_turns-0.3 < current_left_odom-init_left_odom < left_target_turns+0.3) or 
                    (right_target_turns-0.3 < current_right_odom-init_right_odom < right_target_turns+0.3)):
                # Reset the timer if the condition is false
                false_condition_start_time = None
            else:
                if false_condition_start_time is None:
                    false_condition_start_time = brain.timer.time(MSEC)
                elif false_condition_start_time + 100 <= brain.timer.time(MSEC):
                    break
            if time_out > 0 and brain.timer.time(MSEC) - movement_start_time > time_out:
                break
        else:
            if (left_target_turns-0.5 < current_left_odom-init_left_odom < left_target_turns+0.5) or (right_target_turns-0.5 < current_right_odom-init_right_odom < right_target_turns+0.5):
                return
    drivetrain.stop()

# -autonomous code
def auto_red_1():
    intake1.set_velocity(50, PERCENT)
    intake1.spin(FORWARD)
    drivetrain_forward(3, 3, True, 80)
    intake1.set_velocity(80, PERCENT)
    drivetrain_forward(2.9, 2.9, True, 35)
    intake1.set_velocity(40, PERCENT)
    drivetrain_forward(0.85, -0.85, False, 50)
    wait(50, MSEC)
    intake1.set_velocity(100, PERCENT)
    drivetrain_forward(1.92, 1.92, False, 80)
    wait(3, SECONDS)
    intake1.stop()
    drivetrain_forward(-1, -1, True, 100)
    drivetrain_forward(-0.95, 0.95, False, 100)
    intake1.spin(FORWARD)
    drivetrain_forward(2.5, 3, False, 80)
    
    
def auto_red_2():
    intake1.set_velocity(50, PERCENT)
    intake1.spin(FORWARD)
    drivetrain_forward(3, 3, True, 80)
    intake1.set_velocity(80, PERCENT)
    drivetrain_forward(2.9, 2.9, True, 35)
    intake1.set_velocity(40, PERCENT)
    drivetrain_forward(-0.85, 0.85, False, 50)
    wait(50, MSEC)
    intake1.set_velocity(-100, PERCENT)
    drivetrain_forward(1.92, 1.92, False, 80)
    wait(3, SECONDS)
    intake1.stop()
    drivetrain_forward(-1, -1, True, 100)
    drivetrain_forward(0.95, -0.95, False, 100)
    intake1.spin(FORWARD)
    drivetrain_forward(2.5, 3, False, 80)

def auto_blue_1():
    intake1.set_velocity(50, PERCENT)
    intake1.spin(FORWARD)
    drivetrain_forward(3, 3, True, 80)
    intake1.set_velocity(80, PERCENT)
    drivetrain_forward(2.9, 2.9, True, 35)
    intake1.set_velocity(40, PERCENT)
    drivetrain_forward(0.85, -0.85, False, 50)
    wait(50, MSEC)
    intake1.set_velocity(100, PERCENT)
    drivetrain_forward(1.92, 1.92, False, 80)
    wait(3, SECONDS)
    intake1.stop()
    drivetrain_forward(-1, -1, True, 100)
    drivetrain_forward(-0.95, 0.95, False, 100)
    intake1.spin(FORWARD)
    drivetrain_forward(2.5, 3, False, 80)

def auto_blue_2():
    intake1.set_velocity(50, PERCENT)
    intake1.spin(FORWARD)
    drivetrain_forward(3, 3, True, 80)
    intake1.set_velocity(80, PERCENT)
    drivetrain_forward(2.9, 2.9, True, 35)
    intake1.set_velocity(40, PERCENT)
    drivetrain_forward(-0.85, 0.85, False, 50)
    wait(50, MSEC)
    intake1.set_velocity(-100, PERCENT)
    drivetrain_forward(1.92, 1.92, False, 80)
    wait(3, SECONDS)
    intake1.stop()
    drivetrain_forward(-1, -1, True, 100)
    drivetrain_forward(0.95, -0.95, False, 100)
    intake1.spin(FORWARD)
    drivetrain_forward(2.5, 3, False, 80)

def auto_skill():
    drivetrain_forward(1, 1, True, 80)

AUTO_FUNCTIONS = {
    "red_1": auto_red_1,
    "red_2": auto_red_2,
    "blue_1": auto_blue_1,
    "blue_2": auto_blue_2,
    "skill_": auto_skill
}

# autonomous
def autonomous():
    '''
    competition template for autonomous code
    '''
    global team_position
    AUTO_FUNCTIONS[str(team_position)]()
        

# driver control
def user_control():
    '''
    competition template for driver control
    '''
    brain.timer.clear()
    global intake
    # thread all func
    Thread(drivetrain_control)
    #Thread(controller_intake)
    Thread(scoring_angle)
    #Thread(pto_change)
    while True:
        if controller_1.buttonR1.pressing():
            intake1.spin(FORWARD)
            intake2.spin(FORWARD)
            wait_until_release(controller_1.buttonR1.pressing, 5)
        elif controller_1.buttonR2.pressing():
            intake1.spin(REVERSE)
            intake2.spin(REVERSE)
            wait_until_release(controller_1.buttonR2.pressing, 5)
        else:
            intake1.stop()
            intake2.stop()
        wait(20, MSEC)
        wait(20, MSEC)

# ! run after program start
# getting team position
team_position = team_choosing()

Thread(color_sort, (team_position,))

intake1.set_velocity(100, PERCENT)
intake2.set_velocity(100, PERCENT)

# create competition instance
comp = Competition(user_control, autonomous)

