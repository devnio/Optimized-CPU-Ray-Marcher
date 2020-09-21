# Ray Marcher

![HD Mandelbulb](output_examples/out_1080_0.png)

Implementation of a fast ray marcher based on sphere tracing.

This project was part of the ETH Course - Advanced Systems Lab. In a team of 4 members, we had to optimize a specific algorithm following some guidelines learned in class.

[Here](https://github.com/devnio/Optimized-CPU-Ray-Marcher/blob/master/49_report.pdf) is the final report.

# How to compile and run
### Dependencies
No dependencies are required to run our raymarcher. 
Everything is self-contained.

### Compile
First compile the project by running these commands from the root directory:
```
cd build
make
```

A file called `raymarcher` will be created.

![Inception scene using a repeat modifier](output_examples/inception_0.png)

### Run
In the build folder run `./raymarcher`.
After the execution the output will be placed into the `output` directory.

### Compile and Run
If you want to simply compile and run all the predefined scenes, then run the following:

On UNIX:
```console
team049$ cd build
team049/build$ bash run_raymarcher.sh 
```

### Scenes
A Scene is composed of geometric shapes that are each defined by a *signed distance function*.
The combination of all these shapes creates a *signed distance space*. Our raymarcher algorithm is then applied on this space.

The creation of a scene is defined via a JSON file that gets parsed via a custom JSON parser.
With this pipeline we built you can create and customize any scene you want.

The currently available scenes are in the team049/scenes/ folder.

![Scene containing all the sdfs we implemented](output_examples/scene0.png)


## Authors
**Nihat Isik - (github: devnio)**

**Qais El Okaili - (github: Xemno)**

**Alexandre Cavaleri - (github: cavEpfl)** 

**David Graf  - (github: fxd24)**
