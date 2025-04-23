package com.example.flagapp

import android.app.Application

class MainApplication : Application() {

    override fun onCreate() {
        super.onCreate()
        // initialize Amplify when application is starting
        Backend.initialize(applicationContext)
    }
}