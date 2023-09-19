import threading
import tkinter as tk
from tkinter import ttk

from eight_puzzle_binding import *
from search_alg_binding import *

root = tk.Tk()
root.title('CS3642 Assignment Two')
root.geometry('400x200')

# Option menu to select search algorithm.
chosen_search_alg_str = tk.StringVar(root, 'DFS')
search_alg_options_menu = tk.OptionMenu(root, chosen_search_alg_str, 'DFS', 'BFS', 'UCS', 'A*')
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

def run_search_alg():
    search_alg_options_menu.config(state='disabled')
    run_search_alg_button.config(state='disabled')

    # Create random start node and run search algorithm.
    result = SearchResult()
    start_node = generate_random_start_node().contents

    alg = chosen_search_alg_str.get()
    if alg == 'DFS':
        depth_first_search(start_node, result)
    elif alg == 'BFS':
        breadth_first_search(start_node, result)
    elif alg == 'UCS':
        uniform_cost_search(start_node, result)
    elif alg == 'A*':
        a_star_search(start_node, result)
    
    # Update result labels.
    if result.trace:
        num_nodes_visited_str.set(f'# Nodes visited: {result.num_nodes_visited}')
        moves_to_goal_str.set(f'Moves to goal: {result.trace_size - 1}')

    free_search_result(result)

    search_alg_options_menu.config(state='normal')
    run_search_alg_button.config(state='active')

# Button to run search algorithm.
run_search_alg_button = tk.Button(root, text='Run Search Algorithm', 
                                  command=lambda: threading.Thread(target=run_search_alg).start())
run_search_alg_button.config(anchor='w')
run_search_alg_button.grid(row=0, column=1, sticky='w')

root.mainloop()