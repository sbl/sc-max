{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 650.0, 44.0, 643.0, 696.0 ],
		"bglocked" : 0,
		"defrect" : [ 650.0, 44.0, 643.0, 696.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Verdana",
		"gridonopen" : 0,
		"gridsize" : [ 5.0, 5.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "softclip",
					"presentation_rect" : [ 59.0, 485.0, 0.0, 0.0 ],
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 57.0, 483.0, 52.0, 19.0 ],
					"id" : "obj-30",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tanh~",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 14.0, 484.0, 41.0, 19.0 ],
					"id" : "obj-19",
					"outlettype" : [ "signal" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Try some presets:",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 14.0, 190.0, 102.0, 19.0 ],
					"id" : "obj-40",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 1",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 1,
					"hidden" : 1,
					"patching_rect" : [ 104.0, 211.0, 68.0, 19.0 ],
					"id" : "obj-24",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 495.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-41",
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "1st arg: number of control points (this defaults to 12) 1-n\n\nsc.gendy1 understands attributes (check inspector)",
					"linecount" : 3,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 103.0, 340.0, 367.0, 43.0 ],
					"id" : "obj-39",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"minimum" : 0.0,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"maximum" : 1.0,
					"numoutlets" : 2,
					"patching_rect" : [ 422.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-36",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"minimum" : 0.0,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"maximum" : 1.0,
					"numoutlets" : 2,
					"patching_rect" : [ 347.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-35",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 289.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-25",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "preset",
					"numinlets" : 1,
					"numoutlets" : 4,
					"patching_rect" : [ 14.0, 212.0, 89.0, 16.0 ],
					"id" : "obj-22",
					"outlettype" : [ "preset", "int", "preset", "int" ],
					"preset_data" : [ 						{
							"number" : 1,
							"data" : [ 5, "obj-41", "number", "int", 12, 5, "obj-36", "flonum", "float", 0.5, 5, "obj-35", "flonum", "float", 0.5, 5, "obj-25", "flonum", "float", 440.0, 5, "obj-27", "flonum", "float", 1.0, 5, "obj-21", "flonum", "float", 1.0, 5, "obj-23", "number", "int", 1, 5, "obj-17", "number", "int", 1 ]
						}
, 						{
							"number" : 2,
							"data" : [ 5, "obj-41", "number", "int", 12, 5, "obj-36", "flonum", "float", 0.1, 5, "obj-35", "flonum", "float", 0.03, 5, "obj-25", "flonum", "float", 55.0, 5, "obj-27", "flonum", "float", 0.7, 5, "obj-21", "flonum", "float", 0.3, 5, "obj-23", "number", "int", 2, 5, "obj-17", "number", "int", 1 ]
						}
, 						{
							"number" : 3,
							"data" : [ 5, "obj-41", "number", "int", 12, 5, "obj-36", "flonum", "float", 0.883, 5, "obj-35", "flonum", "float", 0.259, 5, "obj-25", "flonum", "float", 42.0, 5, "obj-27", "flonum", "float", 0.5, 5, "obj-21", "flonum", "float", 0.97, 5, "obj-23", "number", "int", 6, 5, "obj-17", "number", "int", 1 ]
						}
, 						{
							"number" : 4,
							"data" : [ 5, "obj-41", "number", "int", 8, 5, "obj-36", "flonum", "float", 0.0, 5, "obj-35", "flonum", "float", 0.069, 5, "obj-25", "flonum", "float", 5670.0, 5, "obj-27", "flonum", "float", 0.441, 5, "obj-21", "flonum", "float", 0.076, 5, "obj-23", "number", "int", 6, 5, "obj-17", "number", "int", 1 ]
						}
, 						{
							"number" : 5,
							"data" : [ 5, "obj-41", "number", "int", 12, 5, "obj-36", "flonum", "float", 0.76, 5, "obj-35", "flonum", "float", 0.01, 5, "obj-25", "flonum", "float", 100.0, 5, "obj-27", "flonum", "float", 0.66, 5, "obj-21", "flonum", "float", 0.455, 5, "obj-23", "number", "int", 2, 5, "obj-17", "number", "int", 6 ]
						}
 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "dcblock",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 177.0, 403.0, 52.0, 19.0 ],
					"id" : "obj-28",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "biquad~ 1. -1. 0. -0.9997 0.",
					"fontname" : "Verdana",
					"numinlets" : 6,
					"numoutlets" : 1,
					"patching_rect" : [ 14.0, 404.0, 161.0, 19.0 ],
					"id" : "obj-29",
					"outlettype" : [ "signal" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"minimum" : 0.00001,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"maximum" : 1.0,
					"numoutlets" : 2,
					"patching_rect" : [ 217.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-27",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "0.0001 - 1",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 177.0, 248.0, 64.0, 19.0 ],
					"id" : "obj-26",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"minimum" : 0.00001,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"maximum" : 1.0,
					"numoutlets" : 2,
					"patching_rect" : [ 145.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-21",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"minimum" : 0,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"maximum" : 6,
					"numoutlets" : 2,
					"patching_rect" : [ 82.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-23",
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "(values between 0-6)",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 17.0, 246.0, 119.0, 19.0 ],
					"id" : "obj-20",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"minimum" : 0,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"maximum" : 6,
					"numoutlets" : 2,
					"patching_rect" : [ 14.0, 271.0, 50.0, 19.0 ],
					"id" : "obj-17",
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pcontrol",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 1,
					"hidden" : 1,
					"patching_rect" : [ 91.0, 154.0, 51.0, 19.0 ],
					"id" : "obj-6",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p details",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"hidden" : 1,
					"patching_rect" : [ 145.0, 155.0, 53.0, 19.0 ],
					"id" : "obj-5",
					"fontsize" : 10.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 188.0, 48.0, 693.0, 456.0 ],
						"bglocked" : 0,
						"defrect" : [ 188.0, 48.0, 693.0, 456.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "ampdist - Choice of probability distribution for the next perturbation of the amplitude of a control point. \n\nThe distributions are (adapted from the GENDYN program in Formalized Music):\n0- LINEAR\n1- CAUCHY\n2- LOGIST\n3- HYPERBCOS\n4- ARCSINE\n5- EXPON\n6- SINUS\nWhere the sinus (Xenakis' name) is in this implementation taken as sampling from a third party oscillator. See example below.\n\ndurdist- Choice of distribution for the perturbation of the current inter control point duration. \n\nadparam- A parameter for the shape of the amplitude probability distribution, requires values in the range 0.0001 to 1 (there are safety checks in the code so don't worry too much if you want to modulate!)\n\nddparam- A parameter for the shape of the duration probability distribution, requires values in the range 0.0001 to 1\n\nfreq- Oscillation frequency.\n\nampscale- Normally 0.0 to 1.0, multiplier for the distribution's delta value for amplitude. An ampscale of 1.0 allows the full range of  -1 to 1 for a change of amplitude.\n\ndurscale- Normally 0.0 to 1.0, multiplier for the distribution's delta value for duration. An ampscale of 1.0 allows the full range of  -1 to 1 for a change of duration.\n\ninitCPs- Initialise the number of control points in the memory. Xenakis specifies 12. There would be this number of control points per cycle of the oscillator, though the oscillator's period will constantly change due to the duration distribution. \n\nknum- Current number of utilised control points, allows modulation.  ",
									"linecount" : 34,
									"fontname" : "Verdana Bold",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 23.0, 11.0, 638.0, 420.0 ],
									"id" : "obj-13",
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"hidden" : 1,
									"patching_rect" : [ 6.0, 246.0, 25.0, 25.0 ],
									"id" : "obj-3",
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 260.0, 473.0, 315.0, 128.0 ],
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.gendy3~ 12",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 14.0, 351.0, 86.0, 19.0 ],
					"id" : "obj-3",
					"outlettype" : [ "signal" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.4",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 14.0, 508.0, 45.0, 19.0 ],
					"id" : "obj-2",
					"outlettype" : [ "signal" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 641.0, 279.0, 43.0 ],
					"id" : "obj-12",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.gendy3~",
					"fontname" : "Arial",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 16.0, 125.0, 25.0 ],
					"id" : "obj-7",
					"fontface" : 3,
					"fontsize" : 16.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 14.0, 549.0, 45.0, 45.0 ],
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 11.0, 134.0, 34.0 ],
					"id" : "obj-10",
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 45.0, 157.0, 50.0, 17.0 ],
					"id" : "obj-8",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Documentation is from SC Helpfiles. \nSC implementation by Nick Collins (sicklincoln.org)\n\nSee Gendy1 help file for background. This variant of GENDYN normalises the durations in each period to force oscillation at the desired pitch. The breakpoints still get perturbed as in Gendy1. \n\nThere is some glitching in the oscillator caused by the stochastic effects: control points as they vary cause big local jumps of amplitude. Put ampscale and durscale low to minimise this. \n\nMore: ",
					"linecount" : 10,
					"fontname" : "Verdana",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 46.0, 498.0, 128.0 ],
					"id" : "obj-9",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ampdist $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 14.0, 303.0, 65.0, 17.0 ],
					"id" : "obj-13",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "durdist $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 82.0, 303.0, 60.0, 17.0 ],
					"id" : "obj-15",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "adparam $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 145.0, 303.0, 69.0, 17.0 ],
					"id" : "obj-16",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ddparam $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 217.0, 303.0, 69.0, 17.0 ],
					"id" : "obj-18",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "freq $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 289.0, 303.0, 47.0, 17.0 ],
					"id" : "obj-11",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ampscale $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 347.0, 303.0, 72.0, 17.0 ],
					"id" : "obj-32",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "durscale $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 422.0, 303.0, 67.0, 17.0 ],
					"id" : "obj-33",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "knum $1",
					"fontname" : "Verdana",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 495.0, 303.0, 53.0, 17.0 ],
					"id" : "obj-34",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 504.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 298.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 91.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 23.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 154.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 226.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 356.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 431.5, 326.0, 23.5, 326.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 23.5, 435.0, 42.0, 435.0, 42.0, 443.0, 63.0, 436.0, 63.0, 446.0, 79.0, 446.0, 79.0, 450.0, 100.0, 450.0, 110.0, 463.0, 121.0, 457.0, 121.0, 470.0, 136.0, 459.0, 142.0, 466.0, 167.0, 449.0, 167.0, 457.0, 174.0, 457.0, 174.0, 453.0, 186.0, 475.0, 205.0, 457.0, 213.0, 470.0, 217.0, 470.0, 222.0, 470.0, 237.0, 458.0, 237.0, 469.0, 258.0, 459.0, 258.0, 465.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-36", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
