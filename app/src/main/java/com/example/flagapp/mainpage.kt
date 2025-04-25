package com.example.flagapp

import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.view.View
import android.widget.CheckBox
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.ui.AppBarConfiguration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.android.volley.*
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.androidnetworking.AndroidNetworking
import com.androidnetworking.error.ANError
import com.androidnetworking.interfaces.StringRequestListener
import okhttp3.internal.http2.Http2Connection


class mainpage : AppCompatActivity() {
    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: mainpage

    override fun onCreate(savedInstanceState: Bundle?) {
        AndroidNetworking.get("http://172.28.7.218/control?action=init&flag=1")
            .build()
            .getAsString(object : StringRequestListener {
                override fun onResponse(response: String) {
                    val delay = Handler()
                    delay.postDelayed({

                    }, 2500)

                    Toast.makeText(
                        applicationContext,
                        "Done: " + response.replace("\n", "") + "!",
                        Toast.LENGTH_LONG
                    )
                }

                override fun onError(anError: ANError) {
                    Toast.makeText(
                        applicationContext,
                        anError.errorBody,
                        Toast.LENGTH_SHORT
                    ).show()
                }
            })
        super.onCreate(savedInstanceState)

        AndroidNetworking.initialize(applicationContext);

        setContentView(R.layout.mainpage)
        val recyclerview = findViewById<RecyclerView>(R.id.recyclerview)
        recyclerview.layoutManager = LinearLayoutManager(this)
        val data = ArrayList<ItemsViewModel>().apply {
            add(ItemsViewModel(false, "East Campus Flag", "Ground", View.generateViewId()))
            add(ItemsViewModel(false, "West Campus Flag", "Ground", View.generateViewId()))
            add(ItemsViewModel(false, "Football Stadium", "Ground", View.generateViewId()))
            add(ItemsViewModel(false, "Location 4", "Ground", View.generateViewId()))
        }

        //multiple different IP addresses to represent the MQTT, actually will be using
        //one IP address but likely different part, this is just for placeholding
        val IPS = ArrayList<String>();
        IPS.add("1");
        IPS.add("2");
        IPS.add("3");
        IPS.add("4");

        val adapter = CustomAdapter(data, IPS, applicationContext)
        recyclerview.adapter = adapter

        val button = findViewById<View>(R.id.button4)
        val button2 = findViewById<View>(R.id.button3)
        val button3 = findViewById<View>(R.id.button5)
        button.setOnClickListener {
            val handler3 = Handler()
            button.setBackgroundColor(getResources().getColor(R.color.darkpink))
            button2.setBackgroundColor(getResources().getColor(R.color.pink))
            button3.setBackgroundColor(getResources().getColor(R.color.pink))
            handler3.postDelayed({
                val i = Intent(this@mainpage, battery::class.java)
                startActivity(i)
            }, 500)
        }
        button3.setOnClickListener {
            val handler3 = Handler()
            button3.setBackgroundColor(getResources().getColor(R.color.darkpink))
            button2.setBackgroundColor(getResources().getColor(R.color.pink))
            button.setBackgroundColor(getResources().getColor(R.color.pink))
            handler3.postDelayed({
                val i = Intent(this@mainpage, profile::class.java)
                startActivity(i)
            }, 500)
        }

        val upbutton = findViewById<View>(R.id.simpleCheckBox2) as CheckBox
        val downbutton = findViewById<View>(R.id.simpleCheckBox) as CheckBox

        upbutton.setOnClickListener {
            if (upbutton.isChecked()) {
                downbutton.setChecked(false)
                AndroidNetworking.get("http://172.28.7.218/control?action=raise&flag=1")
                    .build()
                    .getAsString(object : StringRequestListener {
                        override fun onResponse(response: String) {
                            val delay = Handler()
                            delay.postDelayed({

                            }, 2500)

                            Toast.makeText(
                                applicationContext,
                                "Done: " + response.replace("\n", "") + "!",
                                Toast.LENGTH_LONG
                            ).show()
                        }

                        override fun onError(anError: ANError) {
                            Toast.makeText(
                                applicationContext,
                                anError.errorBody,
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    })
                AndroidNetworking.get("http://172.28.7.218/control?action=raise&flag=2")
                    .build()
                    .getAsString(object : StringRequestListener {
                        override fun onResponse(response: String) {
                            val delay = Handler()
                            delay.postDelayed({

                            }, 2500)

                            Toast.makeText(
                                applicationContext,
                                "Done: " + response.replace("\n", "") + "!",
                                Toast.LENGTH_LONG
                            ).show()
                        }

                        override fun onError(anError: ANError) {
                            Toast.makeText(
                                applicationContext,
                                anError.errorBody,
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    })

                /* val delay = Handler()
                for(i in 0..3)
                    adapter.updateItem(data, i, "loading")
                delay.postDelayed({
                    for(i in 0..3)
                        adapter.updateItem(data, i, "full height")
                    for(i in 0..3)
                        adapter.updateItem2(data, i, true)
                }, 2500)
            }
            else {
                val delay = Handler()
                for(i in 0..3)
                    adapter.updateItem(data, i, "loading")
                delay.postDelayed({
                    for(i in 0..3)
                        adapter.updateItem(data, i, "ground")
                    for(i in 0..3)
                        adapter.updateItem2(data, i, false)
                    Toast.makeText(applicationContext, "Done!", Toast.LENGTH_LONG).show()
                }, 2500)
                 */
            }
        }

        downbutton.setOnClickListener {
            if (downbutton.isChecked()) {
                upbutton.setChecked(false)
                AndroidNetworking.get("http://172.28.7.218/control?action=lower&flag=1")
                    .build()
                    .getAsString(object : StringRequestListener {
                        override fun onResponse(response: String) {
                            val delay = Handler()
                            delay.postDelayed({

                            }, 2500)

                            Toast.makeText(
                                applicationContext,
                                "Done: " + response.replace("\n", "") + "!",
                                Toast.LENGTH_LONG
                            ).show()
                        }

                        override fun onError(anError: ANError) {
                            Toast.makeText(
                                applicationContext,
                                anError.errorBody,
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    })
                AndroidNetworking.get("http://172.28.7.218/control?action=lower&flag=2")
                    .build()
                    .getAsString(object : StringRequestListener {
                        override fun onResponse(response: String) {
                            val delay = Handler()
                            delay.postDelayed({

                            }, 2500)

                            Toast.makeText(
                                applicationContext,
                                "Done: " + response.replace("\n", "") + "!",
                                Toast.LENGTH_LONG
                            ).show()
                        }

                        override fun onError(anError: ANError) {
                            Toast.makeText(
                                applicationContext,
                                anError.errorBody,
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    })

                for(i in 0..3)
                    adapter.updateItem(data, i, "loading")
                val delay = Handler()
                delay.postDelayed({
                    for(i in 0..3)
                        adapter.updateItem(data, i, "half-mast")
                    for(i in 0..3)
                        adapter.updateItem2(data, i, true)
                }, 2500)
            }
            else {
                val delay = Handler()
                for(i in 0..3)
                    adapter.updateItem(data, i, "loading")
                delay.postDelayed({
                    for(i in 0..3)
                        adapter.updateItem(data, i, "ground")
                    for(i in 0..3)
                        adapter.updateItem2(data, i, false)
                    Toast.makeText(applicationContext, "Done!", Toast.LENGTH_LONG).show()
                }, 2500)
            }
        }
    }
}