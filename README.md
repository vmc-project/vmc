# Virtual Monte Carlo

The Virtual Monte Carlo (VMC) allows to run different simulation Monte Carlo without changing the user code and therefore the input and output format as well as the geometry and detector response definition.

The core of the VMC is the category of classes **vmc**. It provides a set of interfaces which completely decouple the dependencies between the user code and the concrete Monte Carlo:

- [TVirtualMC](https://vmc-project.github.io/vmc/html/classTVirtualMC.html): Interface to the concrete Monte Carlo program
- [TVirtualMCApplication](https://vmc-project.github.io/vmc/html/classTVirtualMCApplication.html): Interface to the user's Monte Carlo application
- [TVirtualMCStack](https://vmc-project.github.io/vmc/html/classTVirtualMCStack.html): Interface to the particle stack
- [TVirtualMCDecayer](https://vmc-project.github.io/vmc/html/classTVirtualMCDecayer.html): Interface to the external decayer
- [TVirtualMCSensitiveDetector](https://vmc-project.github.io/vmc/html/classTVirtualMCSensitiveDetector.html): Interface to the user's sensitive detector

The implementation of the TVirtualMC interface is provided for two Monte Carlo transport codes, GEANT3 and [Geant4](http://geant4.web.cern.ch/geant4/), with the VMC packages listed below. The implementation for the third Monte Carlo transport code, FLUKA, has been discontinued by the FLUKA team in 2010.

The other three interfaces are implemented in the user application.The user has to implement two mandatory classes: the **MC application** (derived from TVirtualMCApplication) and the **MC stack** (derived from TVirtualMCStack), optionally an external decayer (derived from TVirtualMCDecayer) can be introduced. The user VMC application is independent from concrete transport codes (GEANT3, Geant4, FLUKA). The transport code which will be used for simulation is selected at run time - when processing a ROOT macro where the concrete Monte Carlo is instantiated.

The relationships between the interfaces and their implementations are illustrated in the class diagrams: User MC application , Virtual MC , demonstarting the decoupling between the user code and the concrete transport code.

## VMC and TGeo

The VMC is fully integrated with the Root geometry package, [TGeo](https://root.cern/doc/master/group__Geometry__classes.html), and users can easily define their VMC application with TGeo geometry and this way of geometry definition is recommended for new users.

It is also possible to define geometry via Geant3-like functions defined in the VMC interface, however this way is kept only for backward compatibility and should not be used by new VMC users.

## Available VMCs

### For GEANT3 - geant3
[Geant3 VMC](https://vmc-project.github.io/user-guide/geant3) (C++) is provided within a single package together with GEANT3 (Fortran) - geant3 .

### For Geant4 - geant4_vmc
[Geant4 VMC](https://vmc-project.github.io/user-guide/geant4-vmc) is provided within a package geant4_vmc , that, in difference from geant3, naturally does not include Geant4 itself and you will need the Geant4 installation to run your VMC application with Geant4 VMC.

## Multiple VMCs

Since the development version the simulation can be shared among multiple different engines deriving from [TVirtualMC](https://vmc-project.github.io/vmc/html/classTVirtualMC.html) which are handled by a singleton [TMCManager](https://vmc-project.github.io/vmc/html/classTMCManager.html) object.

See more detailed description in [the dedicated section](https://vmc-project.github.io/user-guide/vmc/multiple-vmc) in the documentation.

## Authors

The concept of Virtual MonteCarlo has been developed by the [ALICE Software Project](http://aliceinfo.cern.ch/Offline/).<br>
Authors: R.Brun <sup>1</sup>, F.Carminati <sup>1</sup>, A. Gheata <sup>1</sup>, I.Hrivnacova <sup>2</sup>, A.Morsch <sup>1</sup>, B.Volkel <sup>1,3</sup>;<br>
<sup>1</sup>European Organization for Nuclear Research (CERN), Geneva, Switzerland;<br>
<sup>2</sup>Universite Paris-Saclay, CNRS/IN2P3, IJCLab, Orsay, France;<br>
<sup>3</sup>Ruprecht-Karls-University Heidelberg, Germany

Contact: root-vmc@cern.ch

VMC pages maintained by: Ivana Hrivnacova <br>
*Last update: 24/02/2020*
