#ifndef MFA_MINIMAL_DENSE_H
#define MFA_MINIMAL_DENSE_H

//file: mfa8Algo.cpp

/*The minimal dense algorithm used
by the DR-planner.
*/

/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "entities/Cluster.h"
#include "entities/Graph.h"
#include "output/DRPrintUtils.h"
#include "algo/MinimalDense.h"
#include "algo/DistributeTree.h"
#include <iostream>

//input initialized CQueue--all the single vertices of the original graph

class MinimalDense {
public:

// this is similar to algorithm dense, algorithm dense is described in paper documentation
    static List<Cluster> & mfaAlgo(
            GlobalState &globalState,
            Graph &graph1,
            List<Cluster> &DRTrees,
            std::ostream &file1,
            std::ostream &file2)
    {
        int i, j, nCluster, extended, numTrees, number, vertexName, pName;
        Cluster A, C;
        Cluster aDRtree;
        Graph F;       //contain distributed vertices & current edges

        //print the graph1 (simplified from graph0)
        file1<<"---------------------------------------------"<< std::endl;
        file1<<"simpilfied graph (merge the multi-edges and delete zero-weight edges)"<< std::endl;
        graph1.output(file1);

        //print the DR Input Trees
        file1<<"---------------------------------------------"<< std::endl;
        numTrees=DRTrees.returnLen();
        file1<< std::endl <<numTrees<<" DRTrees by design decomposition:"<< std::endl;
        for(i=1;i<=numTrees;i++)
        {
            file1<<"---------"<< std::endl;
            file1<<"DRTree "<<i<< std::endl;
            DRPrintUtils::printTree(DRTrees.retrieve(i), file1, 1);
        }
        file1<<"---------------------------------------------"<< std::endl;
        DRTrees=DistributeTree::distributeForest(globalState, DRTrees, F, graph1, file1, file2, 0,0);

        //get trivial clusters from the single left vertices, if possible
        DistributeTree::getTrivial(globalState, DRTrees, graph1);

        //print the DR Output Trees
        file1<<"---------------------------------------------"<< std::endl;
        numTrees       =DRTrees.returnLen();
        file1<< std::endl <<numTrees<< " Final DR output Trees"<< std::endl;
        for(i=1;i<=numTrees;i++)
        {
            file1<<"---------"<<std::endl;
            file1<<"DRTree "<<i<<std::endl;
            DRPrintUtils::printTree(DRTrees.retrieve(i), file1, 1);
        }
        file1<<"---------------------------------------------"<<std::endl;
        std::cout<<std::endl<<numTrees<<" Final DR output Trees"<<std::endl;
        for(i=1;i<=numTrees;i++)
        {
            std::cout<<"---------"<<std::endl;
            std::cout<<"DRTree "<<i<<std::endl;
            DRPrintUtils::printTree(DRTrees.retrieve(i), std::cout, 1);
        }
        std::cout<<"---------------------------------------------"<<std::endl;
        return DRTrees;
    }

    /*********************************************************************\
     *  Note:                                                            *
     *  in F:                                                            *
     *   Label=1 for necessary extended vertices (avoid trivial dense)   *
     *   Label=2,for well constrained extension                          *
     *   Label=3,for over constrained extension                          *
     *   Scan=-1 for inner vertices of new cluster                       *
    \*********************************************************************/

};

#endif