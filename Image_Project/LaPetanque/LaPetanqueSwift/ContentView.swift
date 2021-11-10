//
//  ContentView.swift
//  LaPetanque
//
//  Created by Raphael Braud-Mussi on 08/11/2021.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        Text("\(Wrapper.openCVVersionString())")
            .padding()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
