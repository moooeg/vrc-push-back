# ---------------------------------------------------------------------------- #
#                                                                              #
# 	Module:       main.py                                                      #
# 	Author:       EricGu                                                       #
# 	Created:      5/12/2025, 11:58:20 AM                                       #
# 	Description:  V5 project                                                   #
#                                                                              #
#   Team 75477M 'Frij'                                                         #
#   Push Back 2025-2026                                                        #
#                                                                              #
# ---------------------------------------------------------------------------- #

# --------------------------------------
# Wiring Guide
# Updated: 2025-05-12

# Port 01 : -
# Port 02 : -
# Port 03 : -
# Port 04 : -
# Port 05 : -
# Port 06 : -
# Port 07 : -
# Port 08 : -
# Port 09 : -
# Port 10 : -
# Port 11 : -
# Port 12 : -
# Port 13 : -
# Port 14 : -
# Port 15 : -
# Port 16 : -
# Port 17 : -
# Port 18 : -
# Port 19 : -
# Port 20 : -
# Port 21 : -

# Port A  : -
# Port B  : -
# Port C  : -
# Port D  : -
# Port E  : -
# Port F  : -
# Port G  : -
# Port H  : -
# --------------------------------------

# ---------------------------------------------------------------------------- #
# port define template
# pneumatics = DigitalOut(brain.three_wire_port.a)
# optical = Optical(Ports.PORT3)
# distance = Distance(Ports.PORT4)
# ---------------------------------------------------------------------------- #

# Library imports
from vex import *

brain = Brain()
controller_1 = Controller(PRIMARY)

# ports settings
left_motor_a = Motor(Ports.PORT1, GearSetting.RATIO_6_1, True)
left_motor_b = Motor(Ports.PORT2, GearSetting.RATIO_6_1, True)
left_motor_c = Motor(Ports.PORT3, GearSetting.RATIO_6_1, True)
left_drive_smart = MotorGroup(left_motor_a,  left_motor_b, left_motor_c)

right_motor_a = Motor(Ports.PORT4, GearSetting.RATIO_18_1, False)
right_motor_b = Motor(Ports.PORT5, GearSetting.RATIO_18_1, False)
right_motor_c = Motor(Ports.PORT6, GearSetting.RATIO_18_1, False)
right_drive_smart = MotorGroup(right_motor_a, right_motor_b, right_motor_c)

drivetrain = DriveTrain(left_drive_smart, right_drive_smart, 299.24 , 377.1, 304.8, MM)

left_odom = Rotation(Ports.PORT7, True)
right_odom = Rotation(Ports.PORT8, True)


# !GUI setup
# -SS GUI init
brain.screen.draw_image_from_file("begin.png", 0, 0)

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

# -side selection
def team_choosing() -> str:
    """
    Start choosing team, can use controller button to select or use GUI on brain.

    Returns:
        str: Literal [ "red_1", "red_2", "blue_1", "blue_2", "skill" ]
    """
    # SS GUI init
    brain.screen.draw_image_from_file("begin.png", 0, 0)

    team = ""
    position = ""
    confirmed = False

    while True:
        wait(5, MSEC)

        # exit
        if confirmed:
            brain.screen.draw_image_from_file(team+position+"_confirmed.png", 0, 0)
            return team + position

        # controller
        if controller_1.buttonL1.pressing():
            team = "red"
            position = "_1"
            confirmed = True
        elif controller_1.buttonL2.pressing():
            team = "red"
            position = "_2"
            confirmed = True
        elif controller_1.buttonR1.pressing():
            team = "blue"
            position = "_1"
            confirmed = True
        elif controller_1.buttonR2.pressing():
            team = "blue"
            position = "_2"
            confirmed = True

        # brain
        if brain.screen.pressing():
            x = brain.screen.x_position()
            y = brain.screen.y_position()

            if GUI_BUTTONS_POSITIONS["top"]["1"].pressing(x, y):
                team = "red"
                position = ""
                brain.screen.draw_image_from_file("red_begin.png", 0, 0)
            elif GUI_BUTTONS_POSITIONS["top"]["2"].pressing(x, y):
                team = "blue"
                position = ""
                brain.screen.draw_image_from_file("blue_begin.png", 0, 0)
            elif GUI_BUTTONS_POSITIONS["top"]["3"].pressing(x, y):
                team = "skill"
                position = ""
                brain.screen.draw_image_from_file("skill_begin.png", 0, 0)

            if team:
                if GUI_BUTTONS_POSITIONS["side"]["1"].pressing(x, y):
                    if team == "skill":
                        confirmed = True
                    else:
                        position = "_1"
                        brain.screen.draw_image_from_file(team+"_1.png", 0, 0)
                elif GUI_BUTTONS_POSITIONS["side"]["2"].pressing(x, y) and team != "skill":
                    position = "_2"
                    brain.screen.draw_image_from_file(team+"_2.png", 0, 0)
                elif GUI_BUTTONS_POSITIONS["side"]["3"].pressing(x, y) and position and team != "skill":
                    confirmed = True
            
            wait_until_release(brain.screen.pressing, 50)


# ! All functions
# -misc.
def cprint(_input: Any):
    s = str(_input)
    controller_1.screen.clear_screen()
    controller_1.screen.set_cursor(1,1)
    controller_1.screen.print(s)

def wait_until_release(fn, time) -> None:
    """
    Block until the passed function return False

    Args:
        fn (function): must returm bool
        time: time to wait in ms
    """
    while fn():
        wait(time, MSEC)


# -thread in driver control
def drivetrain_control():
    while True:
        ratio = 1.1  # Bigger the number, less sensitive
        integral_decay_rate = 0.000003  # Rate at which integral decays
        forward = 100 * math.sin(((controller_1.axis3.position()**3) / 636620))
        if controller_1.axis3.position() < 0:
            forward = 0.8 * forward
        rotate_dynamic = (100 / ratio) * math.sin((abs((forward**3)) / 636620)) * math.sin(((controller_1.axis1.position()**3) / 636620))
        rotate_linear = 40 * math.sin(((controller_1.axis1.position()**3) / 636620))
        max_integral_limit = 0.4*rotate_dynamic
        
        # Accumulate integral when joystick is pushed
        if abs(controller_1.axis1.position()) >= 30:
            integral_rotate += rotate_dynamic * integral_decay_rate
            if integral_rotate > 0:
                integral_rotate = min(integral_rotate, max_integral_limit)  # Cap the integral value
            elif integral_rotate < 0:
                integral_rotate = max(integral_rotate, max_integral_limit)
        else:
            integral_rotate = 0  # Reset the integral when joystick is back below 30

        # Add integral component to turning calculation
        if -30 <= forward <= 30:
            left_drive_smart_speed = forward + rotate_linear
            right_drive_smart_speed = forward - rotate_linear
        else:
            # Use the integral component
            left_drive_smart_speed = forward + rotate_dynamic - integral_rotate
            right_drive_smart_speed = forward - rotate_dynamic + integral_rotate

        if abs(left_drive_smart_speed) < 3:
            if left_drive_smart_stopped:
                left_drive_smart.stop()
                left_drive_smart_stopped = 0
        else:
            left_drive_smart_stopped = 1

        if abs(right_drive_smart_speed) < 3:
            if right_drive_smart_stopped:
                right_drive_smart.stop()
                right_drive_smart_stopped = 0
        else:
            right_drive_smart_stopped = 1

        if left_drive_smart_stopped:
            left_drive_smart.set_velocity(left_drive_smart_speed, PERCENT)
            left_drive_smart.spin(FORWARD)

        if right_drive_smart_stopped:
            right_drive_smart.set_velocity(right_drive_smart_speed, PERCENT)
            right_drive_smart.spin(FORWARD)
            
        wait(20, MSEC)


# -autonomous functions
def drivetrain_turn_on_spot(target_turns: float, speed=100, time_out=0):
    movement_start_time = brain.timer.time(MSEC)
    kp = 40
    ki = 0.03
    kd = 0.1
    
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
    right_drive_smart.spin(REVERSE)
    
    while True:
        left_err = target_turns - (current_left_odom - init_left_odom)
        right_err = target_turns - (init_right_odom - current_right_odom)
        
        left_integral = (left_integral + left_err)*0.95
        right_integral = (right_integral + right_err)*0.95
        
        left_derivitive = left_err - left_prev_error
        right_derivitive = right_err - right_prev_error
        
        left_prev_error = left_err
        right_prev_error = right_err
        
        left_speed = (speed/100)*(max(min((kp * left_err) + (ki * left_integral) + (kd * left_derivitive), 100), -100))
        right_speed = (speed/100)*(max(min((kp * right_err) + (ki * right_integral) + (kd * right_derivitive), 100), -100))
        
        left_drive_smart.set_velocity(left_speed, PERCENT)
        right_drive_smart.set_velocity(right_speed, PERCENT)
        
        current_left_odom = left_odom.position(TURNS)
        current_right_odom = right_odom.position(TURNS)
        
        if not ((target_turns - 0.2 < current_left_odom - init_left_odom < target_turns+0.2) or (target_turns - 0.2 < init_right_odom - current_right_odom < target_turns+0.2)):
            # Reset the timer if the condition is false
            false_condition_start_time = None
        else:
            if false_condition_start_time == None:
                false_condition_start_time = brain.timer.time(MSEC)
            elif false_condition_start_time + 100 <= brain.timer.time(MSEC):
                break
        if movement_start_time-brain.timer.time(MSEC) > time_out and time_out > 0:
            break
    drivetrain.stop()
    
def drivetrain_forward(left_target_turns: float, right_target_turns: float, chain_status, speed=100, time_out=0):
    movement_start_time = brain.timer.time(MSEC)
    false_condition_start_time = None
    
    kp = 40
    ki = 0.03
    kd = 0.1
    
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
        right_err = right_target_turns - (current_right_odom-init_right_odom)
        
        left_integral = (left_integral + left_err)*0.95
        right_integral = (right_integral + right_err)*0.95
        
        left_derivitive = left_err - left_prev_error
        right_derivitive = right_err - right_prev_error
        
        left_prev_error = left_err
        right_prev_error = right_err
        
        left_speed = (speed/100)*(max(min((kp * left_err) + (ki * left_integral) + (kd * left_derivitive), 100), -100))
        right_speed = (speed/100)*(max(min((kp * right_err) + (ki * right_integral) + (kd * right_derivitive), 100), -100))
        
        left_drive_smart.set_velocity(left_speed, PERCENT)
        right_drive_smart.set_velocity(right_speed, PERCENT)
        
        current_left_odom = left_odom.position(TURNS)
        current_right_odom = right_odom.position(TURNS)
        if not chain_status:
            if not ((left_target_turns-0.2 < current_left_odom-init_left_odom < left_target_turns+0.2) or 
                    (right_target_turns-0.2 < current_right_odom-init_right_odom < right_target_turns+0.2)):
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

# autonomous functions

def red_1():
    pass

def red_2():
    pass

def blue_1():
    pass

def blue_2():
    pass

def skill():
    pass    

# autonomous
def autonomous():
    if team_position == "red_1":
        red_1()
    if team_position == "red_2":
        red_2()
    if team_position == "blue_1":
        blue_1()
    if team_position == "blue_2":
        blue_2()
    if team_position == "skill":
        skill()

# driver control
def user_control():
    Thread(drivetrain_control)
    while True:
        wait(20, MSEC)

#getting team position
team_position = team_choosing()

# create competition instance
comp = Competition(user_control, autonomous)