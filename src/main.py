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

# =========================
# WIRING GUIDE
# =========================
#  1: intake1 motor (RATIO_18_1, reversed)
#  2: empty
#  3: park_distance sensor (Distance)
#  4: empty
#  5: empty
#  6: empty
#  7: empty
#  8: empty
#  9: empty
# 10: intake3 motor (RATIO_18_1, reversed)
# 11: right_motor_b (RATIO_6_1, reversed)
# 12: color_sensor (Optical)
# 13: right_motor_a (RATIO_6_1, normal)
# 14: right_motor_c (RATIO_6_1, normal)
# 15: left_motor_b (RATIO_6_1, normal)
# 16: left_motor_a (RATIO_6_1, reversed)
# 17: empty
# 18: left_motor_c (RATIO_6_1, reversed)
# 19: left_odom (Rotation, normal)
# 20: right_odom (Rotation, reversed)
#
# Three-wire ports:
#   A: empty
#   B: double_park (True = lift, False = unlift)
#   C: match_load (True = contracted, False = down)
#   D: descorer (True = hold, False = release)
#   E: empty
#   F: empty
#   G: empty
#   H: empty
# =========================


# ! Initialization
from vex import *

brain = Brain()
controller_1 = Controller(PRIMARY)

# ports settings 
left_motor_a = Motor(Ports.PORT16, GearSetting.RATIO_6_1, True)
left_motor_b = Motor(Ports.PORT15, GearSetting.RATIO_6_1, False)
left_motor_c = Motor(Ports.PORT18, GearSetting.RATIO_6_1, True)
left_drive_smart = MotorGroup(left_motor_a,  left_motor_b, left_motor_c)

right_motor_a = Motor(Ports.PORT13, GearSetting.RATIO_6_1, False)
right_motor_b = Motor(Ports.PORT11, GearSetting.RATIO_6_1, True)
right_motor_c = Motor(Ports.PORT14, GearSetting.RATIO_6_1, False)
right_drive_smart = MotorGroup(right_motor_a, right_motor_b, right_motor_c)

drivetrain = DriveTrain(left_drive_smart, right_drive_smart, 299.24, 377.1, 304.8, MM)

intake1 = Motor(Ports.PORT1, GearSetting.RATIO_18_1, True)
intake3 = Motor(Ports.PORT10, GearSetting.RATIO_18_1, True)

color_sensor = Optical(Ports.PORT12)
park_distance = Distance(Ports.PORT3)

left_odom =  Rotation(Ports.PORT19, False)
right_odom = Rotation(Ports.PORT20, True)

descorer = DigitalOut(brain.three_wire_port.d) #true: hold, false: release
double_park = DigitalOut(brain.three_wire_port.b) #true: lift, false: unlift
match_load = DigitalOut(brain.three_wire_port.c) #true: Contracted, false: down

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
            brain.screen.draw_image_from_file("skill_confirmed.png", 0, 0)
            return team_position

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
                        confirmed = True
                        brain.screen.draw_image_from_file("skill_confirmed.png", 0, 0)
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
        fn (function): must return bool
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

def drivetrain_control():
    '''
    Control the drivetrain using the controller
    '''
    left_drive_smart_speed = 0
    right_drive_smart_speed = 0
    
    prev_forward = 0
    MAX_DELTA = 20  # % change per loop
    deadband = 5

    while True:
        axis3 = controller_1.axis3.position()
        axis1 = controller_1.axis1.position()
        
        if abs(axis3) < deadband:
            axis3 = 0
        if abs(axis1) < deadband:
            axis1 = 0
        
        # apply slew limit only to forward
        target_forward = axis3
        delta = target_forward - prev_forward
        if abs(delta) > MAX_DELTA:
            delta = MAX_DELTA if delta > 0 else -MAX_DELTA
        prev_forward += delta
        forward = prev_forward

        # turning remains fully responsive
        rotate = (30 + 1.9 * math.sqrt(abs(forward))) * math.sin(axis1**3 / 636620)

        left_drive_smart_speed = forward + rotate
        right_drive_smart_speed = forward - rotate

        left_drive_smart.set_velocity(left_drive_smart_speed, PERCENT)
        left_drive_smart.spin(FORWARD)
        
        right_drive_smart.set_velocity(right_drive_smart_speed, PERCENT)
        right_drive_smart.spin(FORWARD)

        wait(10, MSEC)

def match_loading():
    while True:
        wait(10, MSEC)
        if controller_1.buttonL2.pressing() and controller_1.buttonR2.pressing():
            match_load.set(True) #down
            descorer.set(False)
        else:
            match_load.set(False)
            
def double_parking():
    global double_park_status
    double_park_status = False
    while True:
        wait(10, MSEC)
        if controller_1.buttonY.pressing():
            double_park_status = True
            intake1.set_velocity(50, PERCENT)
            while not (36 < park_distance.object_distance(MM) < 46):
                intake1.spin(REVERSE)
                if controller_1.buttonB.pressing():
                    double_park.set(False)
                    break
            intake1.stop()
            double_park.set(True)
        if controller_1.buttonB.pressing():
            double_park_status = False
            double_park.set(False)
        
# -autonomous functions
def drivetrain_forward_kalman(left_target_turns: float, right_target_turns: float, chain_status = False, speed=100, time_out=0):
    '''
    Move forward using PID control and Kalman filter to reduce odometry noise
    Args:
        left_target_turns (float): target turns for left motor
        right_target_turns (float): target turns for right motor
        chain_status (bool): True if not the last motion for motion chain, default is False
        speed (int): speed of the motors, default is 100
        time_out (int): time out in ms, default is 0, 0 means no time out
    '''
    movement_start_time = brain.timer.time(MSEC)
    false_condition_start_time = None
    
    kp = 14
    ki = 4
    kd = 0
    
    left_err = 0
    right_err = 0
    
    left_integral = 0
    right_integral = 0
    
    left_derivative = 0
    right_derivative = 0
    
    left_prev_error = 0
    right_prev_error = 0
    
    init_left_odom = left_odom.position(TURNS)
    init_right_odom = right_odom.position(TURNS)
    
    current_left_odom = left_odom.position(TURNS)
    current_right_odom = right_odom.position(TURNS)
    
    left_drive_smart.spin(FORWARD)
    right_drive_smart.spin(FORWARD)
    
    last_time = brain.timer.time(MSEC) 
    
    # Kalman filter parameters
    mesure_err = 0.005  # R
    process_err = 0.001 # Q
    
    left_best_est = init_left_odom
    right_best_est = init_right_odom
    
    left_best_est_err = 0.01
    right_best_est_err = 0.01
    
    left_prev_best_est = left_best_est
    right_prev_best_est = right_best_est
    
    left_prev_best_est_err = left_best_est_err
    right_prev_best_est_err = right_best_est_err

    while True:
        wait(10, MSEC)
        dt = (brain.timer.time(MSEC) - last_time) / 1000.0
        last_time = brain.timer.time(MSEC)
        
        current_left_odom = left_odom.position(TURNS)
        current_right_odom = right_odom.position(TURNS)
        
        # ----- Left Kalman filter -----
        left_best_est = left_prev_best_est
        left_best_est_err = left_prev_best_est_err + process_err
        
        left_Kalman_out = (left_best_est_err) / (left_best_est_err + mesure_err)
        left_best_est = left_best_est + left_Kalman_out * (current_left_odom - left_best_est)
        left_best_est_err = (1 - left_Kalman_out) * (left_best_est_err)
        
        left_prev_best_est = left_best_est
        left_prev_best_est_err = left_best_est_err
        
        # ----- Right Kalman filter -----
        right_best_est = right_prev_best_est
        right_best_est_err = right_prev_best_est_err + process_err
        
        right_Kalman_out = (right_best_est_err) / (right_best_est_err + mesure_err)
        right_best_est = right_best_est + right_Kalman_out * (current_right_odom - right_best_est)
        right_best_est_err = (1 - right_Kalman_out) * (right_best_est_err)
        
        right_prev_best_est = right_best_est
        right_prev_best_est_err = right_best_est_err
        
        # ----- PID -----
        left_err = left_target_turns - (left_best_est - init_left_odom)
        right_err = right_target_turns - (right_best_est - init_right_odom)
        
        left_integral += left_err * dt
        left_integral *= 0.99    
        right_integral += right_err * dt
        right_integral *= 0.99
        
        left_derivative = (left_err - left_prev_error) / dt
        right_derivative = (right_err - right_prev_error) / dt
        
        left_speed = (speed/100) * max(min((kp * left_err) + (ki * left_integral) + (kd * left_derivative), 100), -100)
        right_speed = (speed/100) * max(min((kp * right_err) + (ki * right_integral) + (kd * right_derivative), 100), -100)
        
        left_prev_error = left_err
        right_prev_error = right_err
        
        left_drive_smart.set_velocity(left_speed, PERCENT)
        right_drive_smart.set_velocity(right_speed, PERCENT)
        
        # ----- Exit conditions -----
        if not chain_status:
            if (not (left_target_turns-0.05 < left_best_est-init_left_odom < left_target_turns+0.05) or 
                    not (right_target_turns-0.05 < right_best_est-init_right_odom < right_target_turns+0.05)):
                false_condition_start_time = None
            else:
                if false_condition_start_time is None:
                    false_condition_start_time = brain.timer.time(MSEC)
                elif false_condition_start_time + 100 <= brain.timer.time(MSEC):
                    break
            if time_out > 0 and brain.timer.time(MSEC) - movement_start_time > time_out:
                break
        else:
            if (left_target_turns-0.1 < left_best_est-init_left_odom < left_target_turns+0.1) and (right_target_turns-0.1 < right_best_est-init_right_odom < right_target_turns+0.1):
                return
    
    drivetrain.stop()

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
    ki = 0
    kd = 0.5
    
    left_err = 0
    right_err = 0
    
    left_integral = 0
    right_integral = 0
    
    left_derivative = 0
    right_derivative = 0
    
    left_prev_error = 0
    right_prev_error = 0
    
    init_left_odom = left_odom.position(TURNS)
    init_right_odom = right_odom.position(TURNS)
    
    current_left_odom = left_odom.position(TURNS)
    current_right_odom = right_odom.position(TURNS)
    
    left_drive_smart.spin(FORWARD)
    right_drive_smart.spin(FORWARD)
    
    last_time = brain.timer.time(MSEC) 

    
    while True:
        wait(10, MSEC)
        dt = (brain.timer.time(MSEC) - last_time) / 1000.0
        last_time = brain.timer.time(MSEC)
        left_err = left_target_turns - (current_left_odom - init_left_odom)
        right_err = right_target_turns - (current_right_odom - init_right_odom)
        
        left_integral += left_err*dt
        left_integral *= 0.99 
        right_integral += right_err*dt
        right_integral *= 0.99
        
        left_derivative = (left_err - left_prev_error) / dt
        right_derivative = (right_err - right_prev_error) / dt
        
        left_speed = (speed/100)*(max(min((kp * left_err) + (ki * left_integral) + (kd * left_derivative), 100), -100))
        right_speed = (speed/100)*(max(min((kp * right_err) + (ki * right_integral) + (kd * right_derivative), 100), -100))
        
        left_prev_error = left_err
        right_prev_error = right_err
        
        left_drive_smart.set_velocity(left_speed, PERCENT)
        right_drive_smart.set_velocity(right_speed, PERCENT)
        
        current_left_odom = left_odom.position(TURNS)
        print(current_left_odom) #get actual result for kalman filter tuning
        current_right_odom = right_odom.position(TURNS)
        
        if not chain_status:
            if (not (left_target_turns-0.2 < current_left_odom-init_left_odom < left_target_turns+0.2) or 
                    not (right_target_turns-0.2 < current_right_odom-init_right_odom < right_target_turns+0.2)):
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
            if (left_target_turns-0.3 < current_left_odom-init_left_odom < left_target_turns+0.3) and (right_target_turns-0.3 < current_right_odom-init_right_odom < right_target_turns+0.3):
                return
    drivetrain.stop()

# -autonomous code
def auto_red_1():
    drivetrain_forward(3.1, 3.1, True, 70, 1000)
    intake1.spin(FORWARD, 80, PERCENT)
    Thread(drivetrain_forward,(3, 3, False, 50, 1000))
    wait(200, MSEC)
    match_load.set(True)
    wait(1300, MSEC)
    intake1.stop()
    match_load.set(False)
    drivetrain_forward(-1.45, -1.45, False, 100)
    intake1.spin_for(REVERSE, 0.5, TURNS, wait = False)
    wait(500, MSEC)
    drivetrain_forward(-0.68, 0.68, False, 100)
    drivetrain_forward(4.7, 4.7, False, 100)
    drivetrain_forward(-0.53, 0.53, False, 100)
    drivetrain_forward(-2, -2, False, 90, 800)
    intake1.stop()
    intake1.spin(FORWARD, 100, PERCENT)
    intake3.spin(FORWARD, 100, PERCENT)
    wait(2000, MSEC)
    intake3.stop()
    intake1.spin(FORWARD, 70, PERCENT)
    drivetrain_forward(1, 1, False, 50)
    drivetrain_forward(-2, -2, False, 80, 800)
    match_load.set(True)
    drivetrain_forward(6.5, 6.7, False, 60, 1000)
    drivetrain.drive(FORWARD, 40, PERCENT)
    wait(1500, MSEC)
    drivetrain.stop()
    drivetrain_forward(-6, -6.2, False, 50, 2000)
    intake1.spin(FORWARD, 100, PERCENT)
    intake3.spin(FORWARD, 100, PERCENT)

def auto_red_2(): 
    pass

def auto_blue_1():
    drivetrain_forward(3.1, 3.1, True, 70, 1000)
    intake1.spin(FORWARD, 80, PERCENT)
    Thread(drivetrain_forward,(3, 3, False, 50, 1000))
    wait(200, MSEC)
    match_load.set(True)
    wait(1300, MSEC)
    intake1.stop()
    match_load.set(False)
    drivetrain_forward(-1.45, -1.45, False, 100)
    intake1.spin_for(REVERSE, 0.5, TURNS, wait = False)
    wait(500, MSEC)
    drivetrain_forward(-0.68, 0.68, False, 100)
    drivetrain_forward(4.7, 4.7, False, 100)
    drivetrain_forward(-0.53, 0.53, False, 100)
    drivetrain_forward(-2, -2, False, 90, 800)
    intake1.stop()
    intake1.spin(FORWARD, 100, PERCENT)
    intake3.spin(FORWARD, 100, PERCENT)
    wait(2000, MSEC)
    intake3.stop()
    intake1.spin(FORWARD, 70, PERCENT)
    drivetrain_forward(1, 1, False, 50) 
    drivetrain_forward(-2, -2, False, 80, 800)
    match_load.set(True)
    drivetrain_forward(6.5, 6.7, False, 60, 1000)
    drivetrain.drive(FORWARD, 40, PERCENT)
    wait(1500, MSEC)
    drivetrain.stop()
    drivetrain_forward(-6, -6.2, False, 50, 2000)
    intake1.spin(FORWARD, 100, PERCENT)
    intake3.spin(FORWARD, 100, PERCENT)

def auto_blue_2():
    drivetrain_forward_kalman(3.82,3.82, False, 80)

def auto_skill():
    if 0:
        drivetrain_forward(3.1, 3.1, True, 70, 1000)
        intake1.spin(FORWARD, 100, PERCENT)
        Thread(drivetrain_forward,(3, 3, False, 60, 1000))
        wait(500, MSEC)
        match_load.set(True)
        wait(1000, MSEC)
        intake1.stop()
        match_load.set(False)
        drivetrain_forward(-1.45, -1.45, False, 80, 1000)
        wait(500, MSEC)
        drivetrain_forward(-0.86, 0.86, False, 100, 1000)
        drivetrain_forward(-3.6, -3.5, False, 80, 1000)
        intake1.spin(FORWARD, 100, PERCENT)
        intake3.spin(REVERSE, 30, PERCENT)
        wait(2500, MSEC)
        intake1.spin(REVERSE, 100, PERCENT)
        drivetrain_forward(2, 2 , False, 100)
        drivetrain_forward(-0.55, 0.55, False, 100, 1000)
        intake1.spin(FORWARD, 50, PERCENT)
        drivetrain_forward(4.3, 4.3 , False, 80)
        intake1.stop() 
        wait(500, MSEC)
        intake3.stop()
        drivetrain_forward(-1.5, 1.5, False, 80, 1000)
        drivetrain_forward(-2.5, -2.5, False, 80, 1000)
        while True:
            intake1.set_velocity(60, PERCENT)
            while not (40 < park_distance.object_distance(MM) < 53):
                intake1.spin(REVERSE, 60, PERCENT)
            intake1.stop()
            wait(100, MSEC)
            double_park.set(True)
            break
    else:
        drivetrain_forward(-1, -1, False, 80)
        intake1.spin(FORWARD, 100, PERCENT)
        intake3.spin(FORWARD, 100, PERCENT)
        match_load.set(True)
        wait(500, MSEC)
        drivetrain.drive(FORWARD,100, PERCENT)
        wait(800, MSEC)
        drivetrain.stop()
        drivetrain_forward(-0.4, -0.4, False, 80, 800)
        match_load.set(False)
        
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
    descorer.set(True)
    AUTO_FUNCTIONS[str(team_position)]()
        
# driver control
def user_control():
    
    descorer.set(True)
    '''
    competition template for driver control
    '''
    brain.timer.clear()
    match_load.set(False)
    double_park_status = False
    if team_position.team == "skill":
        print("1")
        drivetrain.drive(REVERSE, 100, PERCENT)
        wait(150, MSEC)
        drivetrain.stop()
    # thread all func
    Thread(drivetrain_control)
    Thread(match_loading)
    #Thread(double_parking)
    while True:
        if controller_1.buttonY.pressing():
            double_park_status = True
            intake1.set_velocity(50, PERCENT)
            while not (36 < park_distance.object_distance(MM) < 46):
                intake1.spin(REVERSE)
                if controller_1.buttonB.pressing():
                    double_park.set(False)
                    break
            intake1.stop()
            double_park.set(True)
        if controller_1.buttonB.pressing():
            double_park_status = False
            double_park.set(False)
        
        if controller_1.buttonR1.pressing():
            intake1.set_velocity(100, PERCENT)
            intake3.set_velocity(100, PERCENT)
            intake1.spin(FORWARD)
            intake3.spin(FORWARD)
            if controller_1.buttonL1.pressing():
                intake3.set_velocity(100)
            elif controller_1.buttonL2.pressing():
                intake3.set_velocity(60)
                intake3.spin(REVERSE)
            else:
                intake3.set_velocity(100)
                intake3.spin(FORWARD)
        elif controller_1.buttonL2.pressing():
            if controller_1.buttonR2.pressing():
                intake1.spin(FORWARD)
            else: 
                descorer.set(True)
        elif controller_1.buttonR2.pressing():
            intake1.spin(REVERSE)
        elif double_park_status == False:
            intake3.stop()
            intake1.stop()
            descorer.set(False)
                
# ! run after program start
# getting team position
team_position = team_choosing()
descorer.set(True)

#Thread(color_sort, (team_position,))

# create competition instance
comp = Competition(user_control, autonomous)


