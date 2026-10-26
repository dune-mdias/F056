#Installing G4
prerequisites: https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/gettingstarted.html#softwarerequirements

git clone https://gitlab.cern.ch/geant4/geant4
export G4INSTALL=$PWD/geant4


# consulting the install guide https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/installguide.html#buildandinstall
mkdir g4_build
cd g4_build
cmake -DCMAKE_INSTALL_PREFIX=${G4INSTALL} -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -DGEANT4_USE_QT=ON -DGEANT_USE_RAYTRACER_X11=ON ../geant4
make -j 16
make install

export G4SYSTEM=Linux-g++

# We need the G4 datasets, too:
cmake -DGEANT4_INSTALL_DATA=ON .
make 
make install


export G4LEDATA=${G4INSTALL}/share/Geant4/data/G4EMLOW8.8
export G4LEVELGAMMADATA=${G4INSTALL}/share/Geant4/data/PhotonEvaporation6.1.2
export G4SAIDXSDATA=${G4INSTALL}/share/Geant4/data/G4SAIDDATA2.0
export G4LIB=${G4INSTALL}/lib
export G4ENSDFSTATEDATA=${G4INSTALL}/share/Geant4/data/G4ENSDFSTATE3.0
export G4PARTICLEXSDATA=${G4INSTALL}/share/Geant4/data/G4PARTICLEXS4.2
export Geant4_DIR=${G4INSTALL}

# install DAWN
cd <your-Programs-dir>
wget http://geant4.kek.jp/~tanaka/src/dawn_3_91a.tgz
tar xfvz dawn_3_91a.tgz
cd dawn_3_91a
make clean && make guiclean
./configure
make
export PATH=${PATH}:<your-Programs-dir>/dawn_3_91a
# export PATH=${PATH}:${HOME}/public/Programs/dawn_3_91a



cd <your-work-dir>
mkdir build
cd build
cmake -DGeant4_DIR=${G4INSTALL}/lib/Geant4-11.4.0 $G4INSTALL/share/Geant4/examples/basic/B2/B2a
make
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${G4INSTALL}/lib

edit vis.mac to comment out OGL and uncomment DAWN
/vis/open DAWNFILE

./exampleB2a

Write on command line, or in the file vis.mac:
/gun/particle geantino
#/gun/particle e-
#/gun/particle proton
#/gun/particle alpha
/gun/energy 100 GeV
#/run/beamOn 1

#Particle colors (default):
#    negative: red
#    neutral: green
#    positive: blue
/vis/modeling/trajectories/list
/vis/modeling/trajectories/create/drawByParticleID
/vis/modeling/trajectories/select drawByParticleID-0

/vis/modeling/trajectories/drawByParticleID-0/set gamma red
/vis/modeling/trajectories/drawByParticleID-0/set gamma green
/run/beamOn 1

edit $G4INSTALL/share/Geant4/examples/basic/B2/B2a/src/DetectorConstruction.cc to change e.g the target size



#Filtering Example

/vis/modeling/trajectories/create/drawByAttribute
/vis/modeling/trajectories/select drawByAttribute-0

/vis/modeling/trajectories/drawByAttribute-0/setAttribute IMag
/vis/modeling/trajectories/drawByAttribute-0/addInterval interval1 0.0 keV 2.5MeV
/vis/modeling/trajectories/drawByAttribute-0/addInterval interval2 2.5 MeV 5 MeV
/vis/modeling/trajectories/drawByAttribute-0/addInterval interval3 5 MeV 7.5 MeV
/vis/modeling/trajectories/drawByAttribute-0/addInterval interval4 7.5 MeV 10 MeV
/vis/modeling/trajectories/drawByAttribute-0/addInterval interval5 10 MeV 12.5 MeV
/vis/modeling/trajectories/drawByAttribute-0/addInterval interval6 12.5 MeV 10000 MeV
/vis/modeling/trajectories/drawByAttribute-0/interval1/setLineColourRGBA 0.8 0 0.8 1
/vis/modeling/trajectories/drawByAttribute-0/interval2/setLineColourRGBA 0.23 0.41 1 1
/vis/modeling/trajectories/drawByAttribute-0/interval3/setLineColourRGBA 0 1 0 1
/vis/modeling/trajectories/drawByAttribute-0/interval4/setLineColourRGBA 1 1 0 1
/vis/modeling/trajectories/drawByAttribute-0/interval5/setLineColourRGBA 1 0.3 0 1
/vis/modeling/trajectories/drawByAttribute-0/interval6/setLineColourRGBA 1 0 0 1
/vis/filtering/trajectories/create/attributeFilter
/vis/filtering/trajectories/attributeFilter-0/setAttribute IMag
/vis/filtering/trajectories/attributeFilter-0/addInterval 2.5 MeV 1000 MeV
/vis/filtering/trajectories/create/particleFilter
/vis/filtering/trajectories/particleFilter-0/add gamma
/run/beamOn 1

