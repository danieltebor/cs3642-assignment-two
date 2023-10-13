import threading
import time
import tkinter as tk
from tkinter import ttk

from eight_puzzle_binding import *
from search_alg_binding import *

root = tk.Tk()
root.title('CS3642 Assignment Two')
root.geometry('400x120')

# Option menu to select search algorithm.
chosen_search_alg_str = tk.StringVar(root, 'A*')
search_alg_options_menu = tk.OptionMenu(root, chosen_search_alg_str, 'A*', 'DFS', 'BFS', 'UCS')
search_alg_options_menu.config(anchor='w', width=8)
search_alg_options_menu.grid(row=0, column=0, sticky='w')

# Variables for showing the result
num_nodes_visited_str = tk.StringVar(root, '# Nodes visited: 0')
num_nodes_visited_label = tk.Label(root, textvariable=num_nodes_visited_str)
num_nodes_visited_label.config(anchor='w')
num_nodes_visited_label.grid(row=1, column=0, sticky='w')

moves_to_goal_str = tk.StringVar(root, 'Moves to goal: 0')
moves_to_goal_label = tk.Label(root, textvariable=moves_to_goal_str)
moves_to_goal_label.config(anchor='w')
moves_to_goal_label.grid(row=2, column=0, sticky='w')

cpu_time_taken_ms_str = tk.StringVar(root, 'CPU time taken (MS): 0.0')
cpu_time_taken_label = tk.Label(root, textvariable=cpu_time_taken_ms_str)
cpu_time_taken_label.config(anchor='w')
cpu_time_taken_label.grid(row=3, column=0, sticky='w')

def run_search_alg():
    search_alg_options_menu.config(state='disabled')
    run_search_alg_button.config(state='disabled')

    # Create random start node and run search algorithm.
    result = SearchResult()
    start_node = generate_random_start_node().contents

    alg = chosen_search_alg_str.get()
    if alg == 'A*':
        a_star_search(start_node, result)
    elif alg == 'DFS':
        depth_first_search(start_node, result)
    elif alg == 'BFS':
        breadth_first_search(start_node, result)
    elif alg == 'UCS':
        uniform_cost_search(start_node, result)
    
    # Update result labels.
    num_nodes_visited_str.set(f'# Nodes visited: {result.num_nodes_visited}')
    moves_to_goal_str.set(f'Moves to goal: {result.trace_size - 1}')
    cpu_time_taken_ms_str.set(f'CPU time taken (MS): {result.cpu_time_taken_ms:.1f}')

    search_alg_options_menu.config(state='normal')
    run_search_alg_button.config(state='active')

# Button to run search algorithm.
run_search_alg_button = tk.Button(root, text='Run Search Algorithm', 
                                  command=lambda: threading.Thread(target=run_search_alg, daemon=True).start())
run_search_alg_button.config(anchor='w')
run_search_alg_button.grid(row=0, column=1, sticky='w')

root.mainloop()

# Algorithm tester
tester_is_enabled = False
num_tests = 1000

def test_search_algs():
    num_nodes_visited_rolling_avg = 0
    trace_sizes_rolling_avg = 0
    cpu_time_taken_ms_rolling_avg = 0
    
    chosen_search_alg = input('Enter search algorithm to test (A*, DFS, BFS, UCS): ')
    
    for i in range(1, num_tests + 1):
        start_node = generate_random_start_node().contents
        result = SearchResult()
        
        if chosen_search_alg == 'A*':
            a_star_search(start_node, result)    
        elif chosen_search_alg == 'DFS':
            depth_first_search(start_node, result)
        elif chosen_search_alg == 'BFS':
            breadth_first_search(start_node, result)
        elif chosen_search_alg == 'UCS':
            uniform_cost_search(start_node, result)
        else:
            break
        
        # Update rolling average
        num_nodes_visited_rolling_avg += (result.num_nodes_visited - num_nodes_visited_rolling_avg) / i
        trace_sizes_rolling_avg += ((result.trace_size - 1) - trace_sizes_rolling_avg) / i
        cpu_time_taken_ms_rolling_avg += (result.cpu_time_taken_ms - cpu_time_taken_ms_rolling_avg) / i

    print(f'Results of {num_tests} tests run for {chosen_search_alg} search algorithm.')
    print(f'Average # Nodes visited: {num_nodes_visited_rolling_avg:.1f}')
    print(f'Average trace size: {trace_sizes_rolling_avg:.1f}')
    print(f'Average CPU time taken (MS): {cpu_time_taken_ms_rolling_avg:.1f}')
    
if tester_is_enabled:
    test_search_algs()