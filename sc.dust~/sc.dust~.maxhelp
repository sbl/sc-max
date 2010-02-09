{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 267.0, 179.0, 640.0, 506.0 ],
		"bglocked" : 0,
		"defrect" : [ 267.0, 179.0, 640.0, 506.0 ],
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
					"maxclass" : "flonum",
					"numinlets" : 1,
					"id" : "obj-6",
					"patching_rect" : [ 235.0, 140.0, 50.0, 19.0 ],
					"numoutlets" : 2,
					"fontname" : "Verdana",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"id" : "obj-4",
					"patching_rect" : [ 180.0, 255.0, 45.0, 45.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"sig" : 0.0,
					"mode" : 2,
					"numinlets" : 2,
					"id" : "obj-3",
					"patching_rect" : [ 175.0, 215.0, 56.0, 19.0 ],
					"numoutlets" : 2,
					"fontname" : "Verdana",
					"outlettype" : [ "signal", "float" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"sig" : 24.0,
					"mode" : 1,
					"numinlets" : 2,
					"id" : "obj-2",
					"patching_rect" : [ 175.0, 140.0, 56.0, 19.0 ],
					"numoutlets" : 2,
					"fontname" : "Verdana",
					"outlettype" : [ "signal", "float" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.dust~",
					"numinlets" : 1,
					"id" : "obj-1",
					"patching_rect" : [ 175.0, 175.0, 54.0, 19.0 ],
					"numoutlets" : 1,
					"fontname" : "Verdana",
					"outlettype" : [ "signal" ],
					"fontsize" : 10.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
