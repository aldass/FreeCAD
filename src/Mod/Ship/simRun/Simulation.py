#***************************************************************************
#*                                                                         *
#*   Copyright (c) 2011, 2012                                              *  
#*   Jose Luis Cercos Pita <jlcercos@gmail.com>                            *  
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU Lesser General Public License (LGPL)    *
#*   as published by the Free Software Foundation; either version 2 of     *
#*   the License, or (at your option) any later version.                   *
#*   for detail see the LICENCE text file.                                 *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU Library General Public License for more details.                  *
#*                                                                         *
#*   You should have received a copy of the GNU Library General Public     *
#*   License along with this program; if not, write to the Free Software   *
#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
#*   USA                                                                   *
#*                                                                         *
#***************************************************************************

from math import *
import threading

# pyOpenCL
import pyopencl as cl
import numpy as np

# FreeCAD
import FreeCAD,FreeCADGui
from FreeCAD import Base, Vector
import Part

# Ship design module
from shipUtils import Paths, Translator, Math

class Singleton(type):
    def __init__(cls, name, bases, dct):
        cls.__instance = None
        type.__init__(cls, name, bases, dct)
 
    def __call__(cls, *args, **kw):
        if cls.__instance is None:
            cls.__instance = type.__call__(cls, *args,**kw)
        return cls.__instance

class FreeCADShipSimulation(threading.Thread):
    __metaclass__ = Singleton
    def __init__ (self, device, endTime, output, simInstance, FSmesh, waves):
        """ Thread constructor.
        @param device Device to use.
        @param endTime Maximum simulation time.
        @param output [Rate,Type] Output rate, Type=0 if FPS, 1 if IPF.
        @param simInstance Simulaation instance.
        @param FSmesh Free surface mesh faces.
        @param waves Waves parameters (A,T,phi,heading)
        """
        threading.Thread.__init__(self)
        # Setup as stopped
        self.active  = False
        # Build OpenCL context and command queue
        self.device  = device
        if self.device == None: # Can't use OpenCL
            self.context = None
            self.queue   = None
        else:
            self.context = cl.Context(devices=[self.device])
            self.queue   = cl.CommandQueue(self.context)
        # Storage data
        self.endTime = endTime
        self.output  = output
        self.sim     = simInstance
        self.FSmesh  = FSmesh
        self.waves   = waves
        
    def run(self):
        """ Runs the simulation.
        """
        self.active = True
        # Simulation stuff
        if self.device == None:
            from Sim import *
        else:
            from clSim import *
        msg = Translator.translate("\t[Sim]: Initializating...\n")
        FreeCAD.Console.PrintMessage(msg)
        init   = simInitialization(self.FSmesh,self.waves,self.context,self.queue)
        matGen = simMatrixGen(self.context,self.queue)
        solver = simComputeSources(self.context,self.queue)
        fsEvol = simFSEvolution(self.context,self.queue)
        A      = init.A
        FS     = init.fs
        waves  = init.waves
        dt     = init.dt
        t      = 0.0
        nx = FS['Nx']
        ny = FS['Ny']
        msg = Translator.translate("\t[Sim]: Iterating...\n")
        FreeCAD.Console.PrintMessage(msg)
        while self.active and t < self.endTime:
            msg = Translator.translate("\t\t[Sim]: Generating linear system matrix...\n")
            FreeCAD.Console.PrintMessage(msg)
            matGen.execute(FS, A)
            msg = Translator.translate("\t\t[Sim]: Solving linear systems...\n")
            FreeCAD.Console.PrintMessage(msg)
            solver.execute(FS, A)
            msg = Translator.translate("\t\t[Sim]: Time integrating...\n")
            FreeCAD.Console.PrintMessage(msg)
            fsEvol.execute(FS, waves, dt, t)
            t = t + dt
            FreeCAD.Console.PrintMessage('t = %g s\n' % (t))
            # Update FreeCAD
            """
            pos = self.sim.FS_Position[:]
            for i in range(0, nx):
                for j in range(0, ny):
                    pos[i*ny+j].z = float(FS['pos'][i,j][2])
            self.sim.FS_Position = pos[:]
            FreeCAD.ActiveDocument.recompute()
            """
        # Set thread as stopped (and prepare it to restarting)
        self.active = False
        threading.Event().set()
        threading.Thread.__init__(self)

    def stop(self):
        """ Call to stop execution.
        """
        self.active = False
        
    def isRunning(self):
        """ Report thread state
        @return True if thread is running, False otherwise.
        """
        return self.active
