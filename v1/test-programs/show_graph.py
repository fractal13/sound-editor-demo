#!/usr/bin/env python3

import sys
import argparse
import logging
import os.path

import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd

def read_data(filename):
    data = pd.read_csv(filename)
    return data

def get_basename(filename):
    root, ext = os.path.splitext(filename)
    dirname, basename = os.path.split(root)
    logging.info("root: {}  ext: {}  dirname: {}  basename: {}".format(root, ext, dirname, basename))
    return basename

def plot_envelope(my_args, data):
    figure_number = 1
    fig = plt.figure(figure_number, figsize=(6, 4))
    fig.suptitle( "Envelope" )
    ax = fig.add_subplot(1, 1, 1)
    ax.plot("sample_number", "amplitude", data=data)
    ax.set_xlabel("time")
    ax.set_ylabel("amplitude")
    ax.locator_params(axis='both', tight=True, nbins=5)
    fig.tight_layout()

    basename = get_basename(my_args.data_file)
    figure_name = "{}-envelope.{}".format(basename, "pdf")
    
    fig.savefig(figure_name)
    plt.close(fig)
    return

def plot_waveform(my_args, data):
    figure_number = 1
    fig = plt.figure(figure_number, figsize=(6, 4))
    fig.suptitle( "Waveform" )
    ax = fig.add_subplot(1, 1, 1)
    ax.plot("sample_number", "amplitude", data=data)
    ax.set_xlabel("time")
    ax.set_ylabel("amplitude")
    ax.locator_params(axis='both', tight=True, nbins=5)
    fig.tight_layout()

    basename = get_basename(my_args.data_file)
    figure_name = "{}-waveform.{}".format(basename, "pdf")
    
    fig.savefig(figure_name)
    plt.close(fig)
    return

def plot_instrument(my_args, data):
    figure_number = 1
    fig = plt.figure(figure_number, figsize=(6, 4))
    fig.suptitle( "Instrument" )
    ax = fig.add_subplot(1, 1, 1)
    ax.plot("sample_number", "amplitude", data=data)
    ax.set_xlabel("time")
    ax.set_ylabel("amplitude")
    ax.locator_params(axis='both', tight=True, nbins=5)
    fig.tight_layout()

    basename = get_basename(my_args.data_file)
    figure_name = "{}-instrument.{}".format(basename, "pdf")
    
    fig.savefig(figure_name)
    plt.close(fig)
    return

def plot_render(my_args, data):
    figure_number = 1
    fig = plt.figure(figure_number, figsize=(6, 4))
    fig.suptitle( "Render" )
    ax = fig.add_subplot(1, 1, 1)
    ax.plot("sample_number", "amplitude", data=data)
    ax.set_xlabel("time")
    ax.set_ylabel("amplitude")
    ax.locator_params(axis='both', tight=True, nbins=5)
    fig.tight_layout()

    basename = get_basename(my_args.data_file)
    figure_name = "{}-render.{}".format(basename, "pdf")
    
    fig.savefig(figure_name)
    plt.close(fig)
    return

def parse_args(argv):
    parser = argparse.ArgumentParser(prog=argv[0], description='Create Data Plots')
    parser.add_argument('action', default='plot-envelope',
                        choices=["plot-envelope", "plot-waveform", "plot-instrument", "plot-render"], 
                        nargs='?', help="desired action")
    parser.add_argument('--data-file',               '-d', default="",    type=str,   help="csv file of data to display")

    my_args = parser.parse_args(argv[1:])

    #
    # Do any special fixes/checks here
    #
    
    return my_args

def main(argv):
    my_args = parse_args(argv)
    logging.basicConfig(level=logging.WARN)

    filename = my_args.data_file
    if os.path.exists(filename) and os.path.isfile(filename):
        data = read_data(filename)

        if my_args.action in ("plot-envelope", ):
            plot_envelope(my_args, data)
        elif my_args.action in ("plot-waveform", ):
            plot_waveform(my_args, data)
        elif my_args.action in ("plot-instrument", ):
            plot_instrument(my_args, data)
        elif my_args.action in ("plot-render", ):
            plot_render(my_args, data)
    else:
        print(filename + " doesn't exist, or is not a file.")
    
    return

if __name__ == "__main__":
    main(sys.argv)
